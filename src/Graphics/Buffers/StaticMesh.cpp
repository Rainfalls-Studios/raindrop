#include <Raindrop/Graphics/Buffers/StaticMesh.hpp>
#include <Raindrop/Graphics/Buffers/Buffer.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <Raindrop/Graphics/Buffers/HostMesh.hpp>
#include <Raindrop/Graphics/Internal/CommandPool.hpp>
#include <Raindrop/Graphics/Internal/CommandBuffer.hpp>
#include <Raindrop/Graphics/Internal/Queue.hpp>
#include <Raindrop/Graphics/Internal/QueueFamily.hpp>

namespace Raindrop::Graphics::Buffers{
	struct StaticMesh::SubmitInfo{
		Internal::CommandPool& commandPool;
		Internal::CommandBuffer& commandBuffer;
		Internal::Queue& queue;

		SubmitInfo(Context& context) : 
			commandPool{context.graphics.internal.commandPools.pool(VK_QUEUE_TRANSFER_BIT, VK_COMMAND_POOL_CREATE_TRANSIENT_BIT)},
			commandBuffer{commandPool.allocate(VK_COMMAND_BUFFER_LEVEL_PRIMARY)},
			queue{context.graphics.internal.queueHandler.get(commandPool.familyIndex()).get(0)} {

		}
	};

	StaticMesh::StaticMesh(Context& context, const HostMesh& mesh) : _context{context}, _layout{mesh.vertexLayout()}{
		SubmitInfo submitInfo(_context);
		createVertexBuffers(mesh, submitInfo);
		createIndexBuffer(mesh, submitInfo);
	}

	StaticMesh::~StaticMesh(){
		for (auto& buffer : _vertexBuffers){
			buffer.reset();
		}
		_indexBuffer.reset();
	}
	
	void StaticMesh::createVertexBuffers(const HostMesh& mesh, SubmitInfo& submitInfo){
		_vertexBuffers.resize(mesh.bindings());

		std::size_t staginSize = 0;
		for (std::size_t i=0; i<mesh.bindings(); i++){
			if (mesh.bindingSize(i) > staginSize){
				staginSize = mesh.bindingSize(i);
			}
		}

		Buffer staginBuffer(_context);
		staginBuffer.allocate(staginSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();

		Internal::CommandBuffer& commandBuffer = submitInfo.commandBuffer;
		Internal::Queue& queue = submitInfo.queue;

		for (std::size_t i=0; i<mesh.bindings(); i++){
			staginBuffer.write(mesh.data(i));
			staginBuffer.flush();

			auto& vertexBuffer = _vertexBuffers[i];

			vertexBuffer = std::make_unique<Buffer>(_context);
			vertexBuffer->allocateInstances(_layout.bindingSize(i), mesh.vertexCount(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			VkBufferCopy region{};
			region.dstOffset = 0;
			region.srcOffset = 0;
			region.size = vertexBuffer->size();

			commandBuffer.begin();
			vkCmdCopyBuffer(commandBuffer.get(), staginBuffer.get(), vertexBuffer->get(), 1, &region);
			commandBuffer.end();

			queue.submit(commandBuffer);
			queue.waitIdle();
		}
	}

	void StaticMesh::createIndexBuffer(const HostMesh& mesh, SubmitInfo& submitInfo){
		if (mesh.indexCount() == 0) return;

		Internal::CommandBuffer& commandBuffer = submitInfo.commandBuffer;
		Internal::Queue& queue = submitInfo.queue;

		Buffer staginBuffer(_context);
		staginBuffer.allocate(mesh.indexSize() * mesh.indexCount(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();
		staginBuffer.write(mesh.indices());
		staginBuffer.flush();

		_indexBuffer = std::make_unique<Buffer>(_context);
		_indexBuffer->allocateInstances(mesh.indexSize(), mesh.indexCount(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


		VkBufferCopy region{};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = mesh.indexSize() * mesh.indexCount();

		commandBuffer.begin();
		vkCmdCopyBuffer(commandBuffer.get(), staginBuffer.get(), _indexBuffer->get(), 1, &region);
		commandBuffer.end();

		queue.submit(commandBuffer);
		queue.waitIdle();
	}

	bool StaticMesh::hasVertexBuffer() const{
		return !_vertexBuffers.empty();
	}

	bool StaticMesh::hasIndexBuffer() const{
		return static_cast<bool>(_indexBuffer);
	}

	const Buffer& StaticMesh::vertexBuffer(std::size_t index) const{
		return *_vertexBuffers[index];
	}

	const Buffer& StaticMesh::indexBuffer() const{
		return *_indexBuffer;	
	}

	std::size_t StaticMesh::vertexBufferCount() const{
		return _vertexBuffers.size();
	}
}