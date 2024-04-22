#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Utils/alignement.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Buffer::Buffer(Context& context) :
		_context{context},
		_memory{VK_NULL_HANDLE},
		_buffer{VK_NULL_HANDLE},
		_mapped{nullptr},
		_instanceCount{0},
		_instanceSize{0},
		_alignmentSize{0},
		_usageFlags{0},
		_memoryPropertyFlags{0}
	{}

	Buffer::~Buffer(){
		unmap();
		free();
	}

	void Buffer::allocate(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		free();

		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;
		
		_instanceSize = instanceSize;
		_instanceCount = instanceCount;
		_usageFlags = usageFlags;
		_memoryPropertyFlags = memoryPropertyFlags;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = instanceSize * instanceCount;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		Exceptions::checkVkCreation<VkBuffer>(
			vkCreateBuffer(device.get(), &bufferInfo, allocationCallbacks, &_buffer),
			"Failed to create buffer"
		);

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.get(), _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, memoryPropertyFlags);

		Exceptions::checkVkCreation<VkDeviceMemory>(
			vkAllocateMemory(device.get(), &allocInfo, allocationCallbacks, &_memory),
			"Failed to allocate buffer memory"
		);

		Exceptions::checkVkOperation<VkBuffer>(
			vkBindBufferMemory(_context.device.get(), _buffer, _memory, 0),
			"Failed to bind buffer memory",
			Exceptions::VulkanOperationType::BINDING
		);
	}

	void Buffer::allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		_alignmentSize = Utils::alignTo(size, minOffsetAlignment);
		allocate(size, 1, usageFlags, memoryPropertyFlags, minOffsetAlignment);
	}

	void Buffer::free(){
		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;

		if (_buffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
			_buffer = VK_NULL_HANDLE;
		}

		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}
	}

	VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset){
		return vkMapMemory(_context.device.get(), _memory, offset, size, 0, &_mapped);
	}

	void Buffer::unmap(){
		if (_mapped){
			vkUnmapMemory(_context.device.get(), _memory);
			_mapped = nullptr;
		}
	}

	void Buffer::writeToBuffer(void *data, VkDeviceSize size, VkDeviceSize offset) {
		if (size == VK_WHOLE_SIZE) {
			memcpy(_mapped, data, _instanceSize * _instanceCount);
		} else {
			char *memOffset = (char *)_mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(_context.device.get(), 1, &mappedRange);
	}

	VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(_context.device.get(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{_buffer, offset, size};
	}

	void Buffer::writeToIndex(void *data, int index) {
		writeToBuffer(data, _instanceSize, index * _alignmentSize);
	}

	VkResult Buffer::flushIndex(int index) {
		return flush(_alignmentSize, index * _alignmentSize);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfoForIndex(int index) {
		return descriptorInfo(_alignmentSize, index * _alignmentSize);
	}

	VkResult Buffer::invalidateIndex(int index) {
		return invalidate(_alignmentSize, index * _alignmentSize);
	}

	VkBuffer Buffer::get(){
		return _buffer;
	}

	VkDeviceMemory Buffer::memory(){
		return _memory;
	}

	void* Buffer::mapped(){
		return _mapped;
	}
}