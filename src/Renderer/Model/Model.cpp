#include <Raindrop/Renderer/Model/Model.hpp>
#include <Raindrop/Renderer/Model/Mesh.hpp>
#include <Raindrop/Renderer/Model/Vertex.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Raindrop::Renderer::Model{
	Model::Model(Context& context, const Path& path) : _context{context}{
		spdlog::info("Loading model \"{}\" ...", path.string());

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
			spdlog::error("Failed to load model \"{}\" : {}", path.string(), importer.GetErrorString());
			throw std::runtime_error("Failed to load model");
		}

		auto directory = path.parent_path();

		if (!scene->HasMeshes()){
			spdlog::error("The model \"{}\" does not contain meshs", path.string());
			throw std::runtime_error("The model has not meshes");
		}


		spdlog::info("Loading model \"{}\" ({} meshes | {} textures) ...", path.string(), scene->mNumMeshes, scene->mNumTextures);

		_meshes.resize(static_cast<std::size_t>(scene->mNumMeshes));
		
		for (std::size_t i=0; i<scene->mNumMeshes; i++){
			const auto& meshData = scene->mMeshes[i];

			if (meshData->mPrimitiveTypes != aiPrimitiveType_TRIANGLE){
				spdlog::error("Cannot load mesh \"{}\" containing another primitive type than triangle", path.string());
				throw std::runtime_error("Invalid primitive type");
			}

			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(_context);

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
			
			if (meshData->HasFaces()){
				static constexpr std::size_t INDEX_COUNT_PER_FACE = 3;
				indices.resize(static_cast<std::size_t>(meshData->mNumFaces) * INDEX_COUNT_PER_FACE);

				for (std::size_t i=0; i<static_cast<std::size_t>(meshData->mNumFaces); i++){
					const auto& faceData = meshData->mFaces[i];

					for (std::size_t j=0; j<INDEX_COUNT_PER_FACE; j++){
						indices[i*INDEX_COUNT_PER_FACE + j] = faceData.mIndices[j];
					}
				}
			}

			vertices.resize(static_cast<std::size_t>(meshData->mNumVertices));
			for (std::size_t i=0; i<static_cast<std::size_t>(meshData->mNumVertices); i++){
				auto& vertex = vertices[i];

				{
					const auto& position = meshData->mVertices[i];
					vertex.position = {
						position.x,
						position.y,
						position.z
					};
				}

				if (meshData->HasVertexColors(0)){
					const auto& color = meshData->mColors[0][i];
					vertex.color = {
						color.r,
						color.g,
						color.b
					};
				}

				if (meshData->HasNormals()){
					const auto& normal = meshData->mNormals[i];
					vertex.normal = {
						normal.x,
						normal.y,
						normal.z
					};
				}

				if (meshData->HasTextureCoords(0)){
					const auto& UV = meshData->mTextureCoords[0][i];
					vertex.UV = {
						UV.x,
						UV.y
					};
				}
			}

			spdlog::info("Mesh \"{}\" from \"{}\" loaded on memory ({} vertices with {} indices)", meshData->mName.C_Str(), path.string(), vertices.size(), indices.size());

			// Now, GPU memory allocation

			auto& device = _context.device;
			auto& allocationCallbacks = _context.allocationCallbacks;

			const std::size_t verticesSize = sizeof(Vertex) * vertices.size();
			const std::size_t indicesSize = sizeof(uint32_t) * indices.size();

			const bool hasIndices = indicesSize != 0;

			VkDeviceSize totalSize = static_cast<VkDeviceSize>(verticesSize + indicesSize);
			
			VkBuffer stagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

			{
				VkBufferCreateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				info.size = totalSize;
				info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &stagingBuffer) != VK_SUCCESS){
					spdlog::error("Failed to create staging buffer !");
					throw std::runtime_error("Failed to create staging buffer");
				}
			}

			{
				VkMemoryRequirements requirements;
				vkGetBufferMemoryRequirements(device.get(), stagingBuffer, &requirements);

				VkMemoryAllocateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				info.allocationSize = requirements.size;
				info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
				
				if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &stagingBufferMemory) != VK_SUCCESS){
					spdlog::error("Failed to allocate staging buffer memory !");
					throw std::runtime_error("Failed to allocate staging buffer memory");
				}

				if (vkBindBufferMemory(device.get(), stagingBuffer, stagingBufferMemory, 0) != VK_SUCCESS){
					spdlog::error("Failed to bind staging buffer memory !");
					throw std::runtime_error("Failed to bind staging buffer memory");
				}
			}

			mesh->allocateVertexBuffer(verticesSize, vertices.size());

			if (hasIndices){
				mesh->allocateIndexBuffer(indicesSize, indices.size());
			}

			VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		
			{
				VkCommandBufferAllocateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
				info.commandBufferCount = 1;
				info.commandPool = _context.commandPools.singleUseTransfert.get();
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

			// update size MUST be within ]0, 65536]
			// update size MUST be a multiple of 4

			static constexpr std::size_t MAX_UPDATE_SIZE = 65536;

			// Here, 'j' is the remaining bytes count
			for (int j=static_cast<int>(verticesSize); j>0; j-=MAX_UPDATE_SIZE){
				std::size_t updateSize = std::min(static_cast<std::size_t>(j), MAX_UPDATE_SIZE);
				std::size_t offset = verticesSize - static_cast<std::size_t>(j);

				std::size_t rest = updateSize % 4;
				offset -= rest;
				updateSize += rest;

				vkCmdUpdateBuffer(
					commandBuffer,
					stagingBuffer,
					offset,
					updateSize,
					reinterpret_cast<char*>(vertices.data()) + offset
				);
			}

			if (hasIndices){
				for (int j=static_cast<int>(indicesSize); j>0; j-=MAX_UPDATE_SIZE){
					std::size_t updateSize = std::min(static_cast<std::size_t>(j), MAX_UPDATE_SIZE);
					std::size_t offset = indicesSize - static_cast<std::size_t>(j);

					std::size_t rest = updateSize % 4;
					offset -= rest;
					updateSize += rest;

					vkCmdUpdateBuffer(
						commandBuffer,
						stagingBuffer,
						verticesSize + offset,
						updateSize,
						reinterpret_cast<char*>(indices.data()) + offset
					);
				}
			}

			{
				VkBufferCopy region;
				region.dstOffset = 0;
				region.srcOffset = 0;
				region.size = verticesSize;

				vkCmdCopyBuffer(
					commandBuffer,
					stagingBuffer,
					mesh->vertexBuffer(),
					1,
					&region
				);
			}

			if (hasIndices){
				VkBufferCopy region;
				region.dstOffset = 0;
				region.srcOffset = verticesSize;
				region.size = indicesSize;

				vkCmdCopyBuffer(
					commandBuffer,
					stagingBuffer,
					mesh->indexBuffer(),
					1,
					&region
				);
			}

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

				if (vkQueueSubmit(_context.queues.transfertQueue(), 1, &info, fence) != VK_SUCCESS){
					spdlog::error("Failed to submit command buffer");
					throw std::runtime_error("Failed to submit command buffer");
				}
			}

			vkWaitForFences(device.get(), 1, &fence, VK_TRUE, UINT64_MAX);
			vkDestroyFence(device.get(), fence, allocationCallbacks);
			vkFreeMemory(device.get(), stagingBufferMemory, allocationCallbacks);
			vkDestroyBuffer(device.get(), stagingBuffer, allocationCallbacks);
			vkFreeCommandBuffers(device.get(), _context.commandPools.singleUseTransfert.get(), 1, &commandBuffer);

			indices.clear();
			vertices.clear();

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
}