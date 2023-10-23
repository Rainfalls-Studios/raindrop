#include <Raindrop/Graphics/Buffers/StaticMesh.hpp>
#include <Raindrop/Graphics/Buffers/Buffer.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
// #include <Raindrop/Graphics/FrameState.hpp>

namespace Raindrop::Graphics::Buffers{
	StaticMesh::StaticMesh(Context& context, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) : _context{context}{
		createVBO(vertices);
		createIBO(indices);
	}

	StaticMesh::~StaticMesh(){
		_vbo.reset();
		_ibo.reset();
	}
	
	void StaticMesh::createVBO(const std::vector<Vertex>& vertices){
		if (vertices.empty()) return;

		Buffer staginBuffer(_context);
		staginBuffer.allocate(sizeof(Vertex) * vertices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();
		staginBuffer.writeToBuffer((void*)vertices.data());
		staginBuffer.flush();

		_vbo = std::make_unique<Buffer>(_context);
		_vbo->allocateInstances(sizeof(Vertex), vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer commandBuffer = _context.transfert.commandPool.beginSingleTime();

		VkBufferCopy region{};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = sizeof(Vertex) * vertices.size();

		vkCmdCopyBuffer(commandBuffer, staginBuffer.get(), _vbo->get(), 1, &region);

		_context.transfert.commandPool.endSingleTime(commandBuffer);
	}

	void StaticMesh::createIBO(const std::vector<uint32_t>& indices){
		if (indices.empty()) return;

		Buffer staginBuffer(_context);
		staginBuffer.allocate(sizeof(uint32_t) * indices.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		staginBuffer.map();
		staginBuffer.write((void*)indices.data());
		staginBuffer.flush();

		_ibo = std::make_unique<Buffer>(_context);
		_ibo->allocateInstances(sizeof(uint32_t), indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer commandBuffer = _context.transfert.commandPool.beginSingleTime();

		VkBufferCopy region{};
		region.dstOffset = 0;
		region.srcOffset = 0;
		region.size = sizeof(uint32_t) * indices.size();

		vkCmdCopyBuffer(commandBuffer, staginBuffer.get(), _ibo->get(), 1, &region);

		_context.transfert.commandPool.endSingleTime(commandBuffer);
	}

	bool StaticMesh::hasVertexBuffer() const{
		return static_cast<bool>(_vbo);
	}

	bool StaticMesh::hasIndexBuffer() const{
		return static_cast<bool>(_ibo);
	}

	const Buffer& StaticMesh::vertexBuffer() const{
		return *_vbo;
	}

	const Buffer& StaticMesh::indexBuffer() const{
		return *_ibo;	
	}
}