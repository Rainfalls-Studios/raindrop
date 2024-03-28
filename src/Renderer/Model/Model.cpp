#include <Raindrop/Renderer/Model/Model.hpp>
#include <Raindrop/Renderer/Model/Mesh.hpp>
#include <Raindrop/Renderer/Model/Vertex.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <Raindrop/Context.hpp>

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Raindrop::Renderer::Model{
	void loadVertices(std::vector<Vertex>& vertices, const aiMesh* mesh){
		vertices.resize(static_cast<std::size_t>(mesh->mNumVertices));
		for (std::size_t i=0; i<static_cast<std::size_t>(mesh->mNumVertices); i++){
			auto& vertex = vertices[i];

			{
				const auto& position = mesh->mVertices[i];
				vertex.position = {
					position.x,
					position.y,
					position.z
				};
			}

			if (mesh->HasVertexColors(0)){
				const auto& color = mesh->mColors[0][i];
				vertex.color = {
					color.r,
					color.g,
					color.b
				};
			}

			if (mesh->HasNormals()){
				const auto& normal = mesh->mNormals[i];
				vertex.normal = {
					normal.x,
					normal.y,
					normal.z
				};
			}

			if (mesh->HasTextureCoords(0)){
				const auto& UV = mesh->mTextureCoords[0][i];
				vertex.UV = {
					UV.x,
					UV.y
				};
			}
		}
	}

	void loadIndices(std::vector<uint32_t>& indices, const aiMesh* mesh){
		if (mesh->HasFaces()){
			static constexpr std::size_t INDEX_COUNT_PER_FACE = 3;
			indices.resize(static_cast<std::size_t>(mesh->mNumFaces) * INDEX_COUNT_PER_FACE);

			for (std::size_t i=0; i<static_cast<std::size_t>(mesh->mNumFaces); i++){
				const auto& faceData = mesh->mFaces[i];

				for (std::size_t j=0; j<INDEX_COUNT_PER_FACE; j++){
					indices[i*INDEX_COUNT_PER_FACE + j] = faceData.mIndices[j];
				}
			}
		}
	}

	void createStagingBuffer(VkBuffer& buffer, VkDeviceMemory& memory, const std::size_t& size, Context& context){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		{
			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = size;
			info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &buffer) != VK_SUCCESS){
				spdlog::error("Failed to create staging buffer !");
				throw std::runtime_error("Failed to create staging buffer");
			}
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), buffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			
			if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &memory) != VK_SUCCESS){
				spdlog::error("Failed to allocate staging buffer memory !");
				throw std::runtime_error("Failed to allocate staging buffer memory");
			}

			if (vkBindBufferMemory(device.get(), buffer, memory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind staging buffer memory !");
				throw std::runtime_error("Failed to bind staging buffer memory");
			}
		}
	}

	void createCommandBuffer(VkCommandBuffer& commandBuffer, Context& context){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		{
			VkCommandBufferAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.commandBufferCount = 1;
			info.commandPool = context.commandPools.singleUseTransfert.get();
			info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

			if (vkAllocateCommandBuffers(device.get(), &info, &commandBuffer) != VK_SUCCESS){
				spdlog::error("Failed to allocate command buffer");
				throw std::runtime_error("Failed to allocate command buffer");
			}
		}

		{
			VkCommandBufferBeginInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			if (vkBeginCommandBuffer(commandBuffer, &info) != VK_SUCCESS){
				spdlog::error("Failed to begin command buffer");
				throw std::runtime_error("Failed to begin command buffer");
			}
		}
	}

	void submitData(const VkCommandBuffer& commandBuffer, std::size_t size, VkBuffer dst, const void* data, std::size_t dstOffset){
		// update size MUST be within ]0, 65536]
		// update size MUST be a multiple of 4

		static constexpr std::size_t MAX_UPDATE_SIZE = 65536;

		// Here, 'j' is the remaining bytes count

		for (int j=static_cast<int>(size); j>0; j-=MAX_UPDATE_SIZE){
			std::size_t updateSize = std::min(static_cast<std::size_t>(j), MAX_UPDATE_SIZE);
			std::size_t offset = size - static_cast<std::size_t>(j);

			std::size_t rest = updateSize % 4;
			offset -= rest;
			updateSize += rest;

			vkCmdUpdateBuffer(
				commandBuffer,
				dst,
				offset + dstOffset,
				updateSize,
				reinterpret_cast<const char*>(data) + offset
			);
		}
	}

	void copyData(const VkCommandBuffer& commandBuffer, VkBuffer src, VkBuffer dst, std::size_t size, std::size_t dstOffset, std::size_t srcOffset){
		VkBufferCopy region;
		region.dstOffset = static_cast<VkDeviceSize>(dstOffset);
		region.srcOffset = static_cast<VkDeviceSize>(srcOffset);
		region.size = static_cast<VkDeviceSize>(size);

		vkCmdCopyBuffer(
			commandBuffer,
			src,
			dst,
			1,
			&region
		);
	}

	void submitAndWait(const VkCommandBuffer& commandBuffer, Context& context){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		VkFence fence = VK_NULL_HANDLE;
		{
			VkFenceCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

			if (vkCreateFence(device.get(), &info, allocationCallbacks, &fence) != VK_SUCCESS){
				spdlog::error("Failed to create submit fence");
				throw std::runtime_error("Failed to create submit fence");
			}
		}

		{
			vkEndCommandBuffer(commandBuffer);
			VkSubmitInfo info{};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 0;
			info.signalSemaphoreCount = 0;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &commandBuffer;

			if (vkQueueSubmit(context.queues.transfertQueue(), 1, &info, fence) != VK_SUCCESS){
				spdlog::error("Failed to submit command buffer");
				throw std::runtime_error("Failed to submit command buffer");
			}
		}

		vkWaitForFences(device.get(), 1, &fence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(device.get(), fence, allocationCallbacks);
	}

	void loadMesh(Mesh& mesh, const aiMesh* data, Context& context){
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		
		loadVertices(vertices, data);
		loadIndices(indices, data);

		spdlog::info("Mesh \"{}\" loaded on memory ({} vertices with {} indices)", data->mName.C_Str(), vertices.size(), indices.size());

		// Now, GPU memory allocation

		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		const std::size_t verticesSize = sizeof(Vertex) * vertices.size();
		const std::size_t indicesSize = sizeof(uint32_t) * indices.size();

		const bool hasIndices = indicesSize != 0;

		VkDeviceSize totalSize = static_cast<VkDeviceSize>(verticesSize + indicesSize);
		
		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

		createStagingBuffer(
			stagingBuffer,
			stagingBufferMemory,
			totalSize,
			context
		);

		mesh.allocateVertexBuffer(verticesSize, vertices.size());

		if (hasIndices){
			mesh.allocateIndexBuffer(indicesSize, indices.size());
		}

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		createCommandBuffer(
			commandBuffer,
			context
		);
		
		submitData(
			commandBuffer,
			verticesSize,
			stagingBuffer,
			vertices.data(),
			0
		);

		if (hasIndices){
			submitData(
				commandBuffer,
				indicesSize,
				stagingBuffer,
				indices.data(),
				verticesSize
			);
		}

		VkBufferMemoryBarrier bufferMemoryBarrier{};
		bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		bufferMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		bufferMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		bufferMemoryBarrier.buffer = stagingBuffer;
		bufferMemoryBarrier.offset = 0;
		bufferMemoryBarrier.size = VK_WHOLE_SIZE;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0,
			nullptr, 
			1, 
			&bufferMemoryBarrier,
			0,
			nullptr
		);

		copyData(
			commandBuffer,
			stagingBuffer,
			mesh.vertexBuffer(),
			verticesSize,
			0,
			0
		);

		if (hasIndices){
			copyData(
				commandBuffer,
				stagingBuffer,
				mesh.indexBuffer(),
				indicesSize,
				0,
				verticesSize
			);
		}

		submitAndWait(
			commandBuffer,
			context
		);

		vkFreeMemory(device.get(), stagingBufferMemory, allocationCallbacks);
		vkDestroyBuffer(device.get(), stagingBuffer, allocationCallbacks);
		vkFreeCommandBuffers(device.get(), context.commandPools.singleUseTransfert.get(), 1, &commandBuffer);

		indices.clear();
		vertices.clear();
	}

	std::string primitiveTypesToStr(unsigned int types){
		std::string output;

		if (types & aiPrimitiveType_LINE) output += " line";
		if (types & aiPrimitiveType_POINT) output += " point";
		if (types & aiPrimitiveType_POLYGON) output += " polygon";
		if (types & aiPrimitiveType_TRIANGLE) output += " triangle";

		return output;
	}

	Model::Model(Context& context, const Path& path) :
			_context{context}
		{
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		spdlog::info("Loading model \"{}\" ...", path.string());

		Assimp::Importer importer;
		const aiScene *baseframebuffer = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

		if (baseframebuffer == nullptr || baseframebuffer->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !baseframebuffer->mRootNode){
			spdlog::error("Failed to load model \"{}\" : {}", path.string(), importer.GetErrorString());
			throw std::runtime_error("Failed to load model");
		}

		auto directory = path.parent_path();

		if (!baseframebuffer->HasMeshes()){
			spdlog::error("The model \"{}\" does not contain meshs", path.string());
			throw std::runtime_error("The model has no meshes");
		}

		spdlog::info("Loading model \"{}\" ({} meshes | {} materials) ...", path.string(), baseframebuffer->mNumMeshes, baseframebuffer->mNumMaterials);

		if (baseframebuffer->HasMaterials()){

			auto& assets = _context.core.assetManager;
			_materials.resize(static_cast<std::size_t>(baseframebuffer->mNumMaterials));

			for (std::size_t i=0; i<baseframebuffer->mNumMaterials; i++){
				const auto& data = baseframebuffer->mMaterials[i];

				Material::Material material(_context);

				data->Get(AI_MATKEY_COLOR_AMBIENT, material.properties.ambientColor);
				data->Get(AI_MATKEY_COLOR_DIFFUSE, material.properties.diffuseColor);
				data->Get(AI_MATKEY_COLOR_SPECULAR, material.properties.specularColor);
				data->Get(AI_MATKEY_SHININESS, material.properties.shininess);

				aiString path;
				if (data->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS){
					material.textures.diffuse = assets.get<Texture::Texture>("Texture", directory / path.C_Str());
				}

				_materials[i] = _context.materials.registerMaterial(material);
			}
		}

		_meshes.resize(static_cast<std::size_t>(baseframebuffer->mNumMeshes));
		
		for (std::size_t i=0; i<baseframebuffer->mNumMeshes; i++){
			const auto& meshData = baseframebuffer->mMeshes[i];

			if (meshData->mPrimitiveTypes != aiPrimitiveType_TRIANGLE){
				spdlog::error("Cannot load mesh \"{}\" containing another primitive type than triangle ({})", meshData->mName.C_Str(), primitiveTypesToStr(meshData->mPrimitiveTypes));
				throw std::runtime_error("Invalid primitive type");
			}

			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(_context);
			
			loadMesh(*mesh, meshData, _context);
			mesh->materialID() = _materials[meshData->mMaterialIndex];
			_meshes[i] = std::move(mesh);
		}
	}

	Model::~Model(){
		_meshes.clear();
	}

	void Model::render(VkCommandBuffer commandBuffer){
		for (auto& mesh : _meshes){
			mesh->render(commandBuffer);
		}
	}

	std::vector<std::unique_ptr<Mesh>>::iterator Model::begin(){
		return _meshes.begin();
	}

	std::vector<std::unique_ptr<Mesh>>::iterator Model::end(){
		return _meshes.end();
	}
}