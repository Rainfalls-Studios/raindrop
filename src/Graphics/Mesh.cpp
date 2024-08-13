#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/MeshData.hpp>
#include <Raindrop/Graphics/CommandBuffer.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Context.hpp>

namespace Raindrop::Graphics{
	std::shared_ptr<Mesh> Mesh::create(Raindrop::Context& context){
		return context.registry.emplace<Mesh>();
	}

	Mesh::Mesh() noexcept :
		_context{nullptr},
		_data{nullptr},
		_indexBuffer{},
		_vertexBuffers{}
	{}

	Mesh::~Mesh(){
		release();
	}


	Mesh::Mesh(Mesh&& other) : 
		_context{nullptr},
		_data{nullptr},
		_indexBuffer{},
		_vertexBuffers{}
	{
		swap(*this, other);
	}

	Mesh& Mesh::operator=(Mesh&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Mesh& A, Mesh& B){
		std::swap(A._context, B._context);
		std::swap(A._data, B._data);
		std::swap(A._indexType, B._indexType);
		std::swap(A._indexBuffer, B._indexBuffer);
		std::swap(A._vertexBuffers, B._vertexBuffers);
	}

	Mesh& Mesh::prepare(Context& context){
		_context = &context;
		return *this;
	}

	void Mesh::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialize a non prepared mesh");
			throw std::runtime_error("The mesh has not been prepared");
		}

		if (!_data){
			_context->logger->warn("Attempt to intiialize a mesh without proper data");
			throw std::runtime_error("The mesh does not have proper data");
		}

		// TODO: async memory submit
		createIndexBuffer();
		createVertexBuffers();

		_layout = _data->getLayout();
		_vertexCount = _data->getVertexCount();

		_data = nullptr;
	}

	void Mesh::createIndexBuffer(){
		std::size_t indexCount = _data->getIndices().size();
		if (indexCount == 0) return;

		if (indexCount < UINT16_MAX){
			_indexType = VK_INDEX_TYPE_UINT16;
		} else {
			_indexType = VK_INDEX_TYPE_UINT32;
		}

		std::size_t bufferSize = indexCount * (_indexType == VK_INDEX_TYPE_UINT16 ? 2 : 4);

		_indexBuffer = std::make_unique<Buffer>();
		_indexBuffer->prepare(*_context)
			.setSize(bufferSize)
			.setSharingMode(VK_SHARING_MODE_EXCLUSIVE)
			.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
			.setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			.allocate();
		
		// TODO: staging memory pool
		Buffer stagingBuffer;
		stagingBuffer.prepare(*_context)
			.setSize(bufferSize)
			.setSharingMode(VK_SHARING_MODE_EXCLUSIVE)
			.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			.allocate();

		auto indices = _data->getIndicesAs(_indexType);

		stagingBuffer.map();

		memcpy(stagingBuffer.getMappedPointer(), indices.data(), bufferSize);

		stagingBuffer.flush();
		stagingBuffer.unmap();
		indices.clear();

		CommandBuffer commandBuffer = _context->transfertCommandPool.allocate(1, VK_COMMAND_BUFFER_LEVEL_PRIMARY).front();
		commandBuffer.beginSingleUse();

		VkBufferCopy region{
			.srcOffset = 0,
			.dstOffset = 0,
			.size = static_cast<VkDeviceSize>(bufferSize)
		};
		vkCmdCopyBuffer(commandBuffer.get(), stagingBuffer.get(), _indexBuffer->get(), 1, &region);

		commandBuffer.end();
		commandBuffer.submit(_context->getDevice().transfetQueue.queue);

		Exceptions::checkVkOperation<VkQueue>(
			vkQueueWaitIdle(_context->getDevice().transfetQueue.queue),
			"Failed to wait for queue to be idle",
			Exceptions::VulkanOperationType::WAIT,
			_context->logger
		);

		_context->transfertCommandPool.free({commandBuffer});
	}

	void Mesh::createVertexBuffers(){
		const auto& buffersData = _data->getBuffers();

		const std::size_t vertexCount = _data->getVertexCount();

		// The number of vertices times to biggest stride
		// Used to create a unique staging buffer that support all vertex buffers
		std::size_t maxSize = vertexCount * std::max_element(
			buffersData.begin(),
			buffersData.end(),
			[](const MeshData::Buffer& A, const MeshData::Buffer& B) -> bool {
				return A.stride < B.stride;
			}
		)->stride;

		// Allocate a command buffer for the next transfers
		CommandBuffer commandBuffer = _context->transfertCommandPool.allocate(1, VK_COMMAND_BUFFER_LEVEL_PRIMARY).front();

		// Allocate the staging buffer that will be used to transfer all the vertices
		Buffer stagingBuffer;
		stagingBuffer.prepare(*_context)
			.setSize(maxSize)
			.setSharingMode(VK_SHARING_MODE_EXCLUSIVE)
			.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			.allocate();

		// The staging buffer will be mapped during the whole process
		stagingBuffer.map();

		// Allocate the buffer vector
		_vertexBuffers.resize(buffersData.size());

		for (std::size_t i=0; i<buffersData.size(); i++){
			auto& buffer = _vertexBuffers[i];
			const auto& data = buffersData[i];

			std::size_t bufferSize = vertexCount * data.stride;

			// copy the vertices into the staging buffer
			memcpy(stagingBuffer.getMappedPointer(), data.data.data(), bufferSize);

			// ensure data coherency
			stagingBuffer.flush();

			// Allocate the vertex buffer
			buffer.prepare(*_context)
				.setSize(data.stride * vertexCount)
				.setSharingMode(VK_SHARING_MODE_EXCLUSIVE)
				.setUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
				.setMemoryProperties(data.memProperties)
				.allocate();
			
			// begin command buffer
			commandBuffer.beginSingleUse();

			// copy the staging buffer into the vertex buffer
			VkBufferCopy region{
				.srcOffset = 0,
				.dstOffset = 0,
				.size = static_cast<VkDeviceSize>(bufferSize)
			};
			vkCmdCopyBuffer(commandBuffer.get(), stagingBuffer.get(), buffer.get(), 1, &region);

			commandBuffer.end();
			commandBuffer.submit(_context->getDevice().transfetQueue.queue);

			Exceptions::checkVkOperation<VkQueue>(
				vkQueueWaitIdle(_context->getDevice().transfetQueue.queue),
				"Failed to wait for queue to be idle",
				Exceptions::VulkanOperationType::WAIT,
				_context->logger
			);
		}

		stagingBuffer.unmap();
		_context->transfertCommandPool.free({commandBuffer});
	}

	void Mesh::release(){
		if (!_context) return;

		_indexBuffer.reset();
		_vertexBuffers.clear();

		_context = nullptr;
	}

	Mesh& Mesh::setData(const MeshData& data){
		_data = &data;
		return *this;
	}

	const VkIndexType& Mesh::getIndexType() const noexcept{
		return _indexType;
	}

	const Buffer* Mesh::getIndexBuffer() const noexcept{
		return _indexBuffer.get();
	}

	const std::vector<Buffer>& Mesh::getVertexBuffers() const noexcept{
		return _vertexBuffers;
	}

	bool Mesh::hasIndexBuffer() const noexcept{
		return _indexBuffer != nullptr;
	}

	const VertexLayout& Mesh::getVertexLayout() const noexcept{
		return _layout;
	}

	const std::size_t& Mesh::getVertexCount() const noexcept{
		return _vertexCount;
	}
}