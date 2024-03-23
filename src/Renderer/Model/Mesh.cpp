#include <Raindrop/Renderer/Model/Mesh.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Model{
	Mesh::Mesh(Context& context) :
		_context{context},
		_vertexBuffer{VK_NULL_HANDLE},
		_vertexMemory{VK_NULL_HANDLE},
		_indexBuffer{VK_NULL_HANDLE},
		_indexMemory{VK_NULL_HANDLE}
	{

	}

	Mesh::~Mesh(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_vertexBuffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _vertexBuffer, allocationCallbacks);
			_vertexBuffer = VK_NULL_HANDLE;
		}

		if (_vertexMemory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _vertexMemory, allocationCallbacks);
			_vertexMemory = VK_NULL_HANDLE;
		}

		if (_indexBuffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _indexBuffer, allocationCallbacks);
			_indexBuffer = VK_NULL_HANDLE;
		}

		if (_indexMemory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _indexMemory, allocationCallbacks);
			_indexMemory = VK_NULL_HANDLE;
		}
	}

	VkBuffer& Mesh::vertexBuffer(){
		return _vertexBuffer;
	}

	VkDeviceMemory& Mesh::vertexMemory(){
		return _vertexMemory;
	}

	VkBuffer& Mesh::indexBuffer(){
		return _indexBuffer;
	}

	VkDeviceMemory& Mesh::indexMemory(){
		return _indexMemory;
	}

	void Mesh::allocateVertexBuffer(const VkDeviceSize& size, const std::size_t& count){
		_vertexCount = count;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		{
			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = size;
			info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &_vertexBuffer) != VK_SUCCESS){
				spdlog::error("Failed to create vertex buffer (size : {})", size);
				throw std::runtime_error("Failed to create vertex buffer");
			}
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), _vertexBuffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &_vertexMemory) != VK_SUCCESS){
				spdlog::error("Failed to allocate vertex buffer memory (size : {})", size);
				throw std::runtime_error("Failed to allocate vertex buffer memory");
			}

			if (vkBindBufferMemory(device.get(), _vertexBuffer, _vertexMemory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind vertex buffer memory !");
				throw std::runtime_error("Failed to bind vertex buffer memory");
			}
		}
	}

	void Mesh::allocateIndexBuffer(const VkDeviceSize& size, const std::size_t& count){
		_indexCount = count;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		{
			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = size;
			info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

			if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &_indexBuffer) != VK_SUCCESS){
				spdlog::error("Failed to create index buffer (size : {})", size);
				throw std::runtime_error("Failed to create index buffer");
			}
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), _indexBuffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &_indexMemory) != VK_SUCCESS){
				spdlog::error("Failed to allocate index buffer memory (size : {})", size);
				throw std::runtime_error("Failed to allocate index buffer memory");
			}

			if (vkBindBufferMemory(device.get(), _indexBuffer, _indexMemory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind index buffer memory !");
				throw std::runtime_error("Failed to bind idnex buffer memory");
			}
		}
	}

	void Mesh::render(VkCommandBuffer commandBuffer){
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_vertexBuffer, &offset);

		if (_indexCount != 0){
			vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
		} else {
			vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
		}
	}

	VkDescriptorSet& Mesh::descriptorSet(){
		return _descriptorSet;
	}
}