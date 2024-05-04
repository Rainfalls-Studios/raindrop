#include <Raindrop/Graphics/Models/Model.hpp>
#include <Raindrop/Graphics/Models/Mesh.hpp>
#include <Raindrop/Graphics/Models/Vertex.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop/Exceptions/ResourceExceptions.hpp>

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Models{
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

			Exceptions::checkVkCreation<VkBuffer>(
				vkCreateBuffer(device.get(), &info, allocationCallbacks, &buffer),
				"Failed to create mesh stagin buffer"
			);
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), buffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

			Exceptions::checkVkCreation<VkDeviceMemory>(
				vkAllocateMemory(device.get(), &info, allocationCallbacks, &memory),
				"Failed to allocate mesh stagin buffer memory"
			);

			Exceptions::checkVkOperation<VkBuffer>(
				vkBindBufferMemory(device.get(), buffer, memory, 0),
				"Failed to bind mesh stagin buffer memory",
				Exceptions::VulkanOperationType::BINDING
			);
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

			Exceptions::checkVkCreation<VkCommandBuffer>(
				vkAllocateCommandBuffers(device.get(), &info, &commandBuffer),
				"Failed to allocate model transfert command buffer"
			);
		}

		{
			VkCommandBufferBeginInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			Exceptions::checkVkOperation<VkCommandBuffer>(
				vkBeginCommandBuffer(commandBuffer, &info),
				"Failed to begin model transfert command buffer",
				Exceptions::VulkanOperationType::BEGIN
			);
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

			Exceptions::checkVkCreation<VkFence>(
				vkCreateFence(device.get(), &info, allocationCallbacks, &fence),
				"Failed to create model transfert command buffer submit fence"
			);
		}

		{

			Exceptions::checkVkOperation<VkCommandBuffer>(
				vkEndCommandBuffer(commandBuffer),
				"Failed to end model transfert command buffer",
				Exceptions::VulkanOperationType::END
			);

			VkSubmitInfo info{};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 0;
			info.signalSemaphoreCount = 0;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &commandBuffer;
			
			Exceptions::checkVkOperation<VkCommandBuffer>(
				vkQueueSubmit(context.queues.transfertQueue(), 1, &info, fence),
				"Failed to submit model transfert command buffer",
				Exceptions::VulkanOperationType::SUBMIT
			);
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
		const aiScene *scene = importer.ReadFile(path.string().c_str(), aiProcess_Triangulate | aiProcess_GenNormals);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			spdlog::error("Failed to load model \"{}\" : {}", path.string(), importer.GetErrorString());
			throw Exceptions::ResourceLoadException(path, "Model", importer.GetErrorString());
		}

		auto directory = path.parent_path();

		if (!scene->HasMeshes()){
			spdlog::error("The model \"{}\" does not contain meshs", path.string());
			throw Exceptions::ResourceLoadException(path, "Model", "scene contains no meshes");
		}

		spdlog::info("Loading model \"{}\" ({} meshes | {} materials) ...", path.string(), scene->mNumMeshes, scene->mNumMaterials);

		if (scene->HasMaterials()){

			auto& assets = _context.core.assetManager;
			_materials.resize(static_cast<std::size_t>(scene->mNumMaterials));

			for (std::size_t i=0; i<scene->mNumMaterials; i++){
				const auto& data = scene->mMaterials[i];

				Materials::Material material(_context);

				data->Get(AI_MATKEY_COLOR_AMBIENT, material.properties.ambientColor);
				data->Get(AI_MATKEY_COLOR_DIFFUSE, material.properties.diffuseColor);
				data->Get(AI_MATKEY_COLOR_SPECULAR, material.properties.specularColor);
				data->Get(AI_MATKEY_SHININESS, material.properties.shininess);

				data->Get(AI_MATKEY_SPECULAR_FACTOR, material.properties.specularFactor);

				aiString path;
				if (data->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS){
					std::replace(
						path.data,
						path.data + path.length,
						'\\',
						'/'
					);

					material.textures.diffuse = assets.getAsset<Textures::Texture>("Texture", directory / path.C_Str());
				}

				_materials[i] = _context.materials.registerMaterial(material);
			}
		}

		_meshes.resize(static_cast<std::size_t>(scene->mNumMeshes));
		
		for (std::size_t i=0; i<scene->mNumMeshes; i++){
			const auto& meshData = scene->mMeshes[i];

			if (!(meshData->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)){
				spdlog::error("Cannot load mesh \"{}\" containing another primitive type than triangle ({})", meshData->mName.C_Str(), primitiveTypesToStr(meshData->mPrimitiveTypes));
				throw Exceptions::ResourceLoadException(path, "Model", "The scene contains not supported primitive types");
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
	
	
	std::size_t Model::meshCount() const{
		return _meshes.size();
	}

	Mesh& Model::mesh(const std::size_t& ID) const{
		assert(ID < meshCount() && "Mesh ID out of bounds");
		return *_meshes[ID];
	}
}