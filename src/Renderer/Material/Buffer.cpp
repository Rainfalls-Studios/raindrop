#include <Raindrop/Graphics/Materials/Buffer.hpp>
#include <Raindrop/Graphics/Materials/Context.hpp>
#include <Raindrop/Graphics/Materials/Material.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Materials{
	Buffer::Buffer(Context& context) : 
			_context{context},
			_buffer{VK_NULL_HANDLE},
			_memory{VK_NULL_HANDLE}
	{
		allocate(_context.instanceCount);
	}

	Buffer::~Buffer(){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_buffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
			_buffer = VK_NULL_HANDLE;
		}

		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}
	}

	void Buffer::allocate(const std::size_t instanceCount){
		_instanceSize = sizeof(Material::Properties);
		_instanceCount = instanceCount;

		{
			const std::size_t alignement = _context.renderer.physicalDevice.limits().minUniformBufferOffsetAlignment;
			const std::size_t rest = _instanceSize % alignement;
			_alignedInstanceSize = rest != 0 ? _instanceSize + (alignement - rest) : _instanceSize;
		}

		spdlog::info("allocating material uniform buffer ... (count : {} | instance size : {} | instance aligned size {})", instanceCount, _instanceSize, _alignedInstanceSize);

		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		{
			std::size_t size = _alignedInstanceSize * _instanceCount;

			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = size;
			info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			

			if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &_buffer) != VK_SUCCESS){
				spdlog::error("Failed to create material uniform buffer ! (size : {})", size);
				throw std::runtime_error("Failed to create material uniform buffer !");
			}
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), _buffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &_memory) != VK_SUCCESS){
				spdlog::error("Failed to allocate material uniform buffer memory !");
				throw std::runtime_error("Failed to allocate material uniform buffer memory");
			}

			if (vkBindBufferMemory(device.get(), _buffer, _memory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind material uniform buffer memory !");
				throw std::runtime_error("Failed to bind material uniform buffer memory");
			}
		}
	}

	VkBuffer Buffer::get() const{
		return _buffer;
	}

	VkDeviceMemory Buffer::memory() const{
		return _memory;
	}

	void Buffer::write(const std::size_t& index, const Material::Properties& data){
		auto& device = _context.renderer.device;
		auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		std::size_t offset = index * _alignedInstanceSize;// sizeof(Material::Properties);
		std::size_t size = sizeof(Material::Properties);

		void* ptr;
		if (vkMapMemory(device.get(), _memory, offset, size, 0, &ptr) != VK_SUCCESS){
			spdlog::error("Failed to map material memory (size : {} | offset : {})", size, offset);
			throw std::runtime_error("Failed to map material memory");
		}

		memcpy(ptr, &data, size);

		vkUnmapMemory(device.get(), _memory);
	}

	std::size_t Buffer::alignedInstanceSize() const{
		return _alignedInstanceSize;
	}

	VkDescriptorBufferInfo Buffer::info(const std::size_t& index){
		return VkDescriptorBufferInfo{
			.buffer = _buffer,
			.offset = _alignedInstanceSize * index,
			.range = _instanceSize
		};
	}
}