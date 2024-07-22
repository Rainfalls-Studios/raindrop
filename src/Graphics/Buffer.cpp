#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Utils/alignement.hpp>

namespace Raindrop::Graphics{
	Buffer::Buffer() noexcept : 
		_context{nullptr},
		_buffer{VK_NULL_HANDLE},
		_memory{VK_NULL_HANDLE},
		_mapped{nullptr}
	{}
	
	Buffer::~Buffer(){
		release();
	}

	void Buffer::prepare(Context& context){
		_context = &context;
	}

	void Buffer::release(){
		if (!_context) return;
		free();
		_context = nullptr;
	}

	void Buffer::allocate(const std::size_t& size, const VkBufferUsageFlags& usage, const VkBufferCreateFlags& flags, const VkMemoryPropertyFlags& memoryFlags, const std::size_t& alignement){
		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;
		
		_size = static_cast<std::size_t>(Utils::alignTo(size, alignement));

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<VkDeviceSize>(_size);
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = flags;

		_context->logger->trace("Creating a graphics buffer...");

		Exceptions::checkVkCreation<VkBuffer>(
			vkCreateBuffer(device.get(), &bufferInfo, allocationCallbacks, &_buffer),
			"Failed to create buffer",
			_context->logger
		);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.get(), _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, memoryFlags);

		_context->logger->trace("Allocating a graphics buffer...");

		Exceptions::checkVkCreation<VkDeviceMemory>(
			vkAllocateMemory(device.get(), &allocInfo, allocationCallbacks, &_memory),
			"Failed to allocate memory",
			_context->logger
		);
		
		_context->logger->trace("Binding buffer and memory...");

		Exceptions::checkVkOperation<VkDeviceMemory>(
			vkBindBufferMemory(device.get(), _buffer, _memory, 0),
			"Failed to bind buffer and memory",
			Exceptions::VulkanOperationType::BINDING,
			_context->logger
		);
	}

	void Buffer::free(){
		unmap();

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;
		
		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}

		if (_buffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
			_buffer = VK_NULL_HANDLE;
		}

		_mapped = nullptr;
	}

	const VkBuffer& Buffer::get() const noexcept{
		return _buffer;
	}

	const VkDeviceMemory& Buffer::getMemory() const noexcept{
		return _memory;
	}

	void Buffer::map(const std::size_t& size, const std::size_t& offset){
		VkDeviceSize deviceSize = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkMapMemory(_context->getDevice().get(), _memory, static_cast<VkDeviceSize>(offset), deviceSize, 0, &_mapped),
			"Failed to map memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}

	void Buffer::unmap(){
		if (_mapped){
			vkUnmapMemory(_context->getDevice().get(), _memory);
			_mapped = nullptr;
		}
	}

	bool Buffer::isMapped() const noexcept{
		return _mapped != nullptr;
	}

	void* Buffer::getMappedPointer() noexcept{
		return _mapped;
	}

	void Buffer::flush(const std::size_t& size, const std::size_t& offset){
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkFlushMappedMemoryRanges(_context->getDevice().get(), 1, &mappedRange),
			"Failed to flush memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}

	void Buffer::invalidate(const std::size_t& size, const std::size_t& offset){
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkInvalidateMappedMemoryRanges(_context->getDevice().get(), 1, &mappedRange),
			"Failed to invalidate memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}

}