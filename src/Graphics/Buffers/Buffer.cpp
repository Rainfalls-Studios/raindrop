#include <Raindrop/Graphics/Buffers/Buffer.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	inline VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment) {
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}

	Buffer::Buffer(Context& context) : _context{context}{}

	Buffer::~Buffer(){
		unmap();
		free();
	}

	void Buffer::allocateInstances(VkDeviceSize instanceSize, uint32_t instanceCount, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		auto& device = _context.graphics.internal.device;
		auto& allocationCallbacks = _context.graphics.allocationCallbacks;

		_context.logger.debug("allocating buffer || instance size : %d, instance count : %d, usageFlags : %x, memoryPropertyFlags : %x, minimal offeset : %d",
			instanceSize,
			instanceCount,
			usageFlags,
			memoryPropertyFlags,
			minOffsetAlignment
		);

		free();
		
		_instanceSize = instanceSize;
		_instanceCount = instanceCount;
		_usageFlags = usageFlags;
		_memoryPropertyFlags = memoryPropertyFlags;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = instanceSize * instanceCount;
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device.get(), &bufferInfo, allocationCallbacks, &_buffer) != VK_SUCCESS){
			_context.logger.error("Failed to create a buffer");
			throw std::runtime_error("Failed to create buffer");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.get(), _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, memoryPropertyFlags);

		if (vkAllocateMemory(device.get(), &allocInfo, nullptr, &_memory) != VK_SUCCESS) {
			_context.logger.error("Failed to allocate a buffer memory");
			throw std::runtime_error("Failed to allocate buffer memory");
		}

		vkBindBufferMemory(device.get(), _buffer, _memory, 0);
	}

	void Buffer::allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		_alignmentSize = getAlignment(_instanceSize, minOffsetAlignment);
		allocateInstances(size, 1, usageFlags, memoryPropertyFlags, minOffsetAlignment);
	}

	void Buffer::free(){
		auto& device = _context.graphics.internal.device;
		auto& allocationCallbacks = _context.graphics.allocationCallbacks;

		if (_buffer) vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
		if (_memory) vkFreeMemory(device.get(), _memory, allocationCallbacks);
	}

	VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset){
		auto& device = _context.graphics.internal.device;
		return vkMapMemory(device.get(), _memory, offset, size, 0, &_mapped);
	}

	void Buffer::unmap(){
		auto& device = _context.graphics.internal.device;
		if (_mapped){
			vkUnmapMemory(device.get(), _memory);
			_mapped = nullptr;
		}
	}

	void Buffer::write(void *data, VkDeviceSize size, VkDeviceSize offset){
		if (size == VK_WHOLE_SIZE) {
			memcpy(_mapped, data, _instanceSize * _instanceCount);
		} else {
			char *memOffset = (char *)_mapped;
			memOffset += offset;
			memcpy(memOffset, data, size);
		}
	}

	VkResult Buffer::flush(VkDeviceSize size, VkDeviceSize offset){
		auto& device = _context.graphics.internal.device;

		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkFlushMappedMemoryRanges(device.get(), 1, &mappedRange);
	}

	VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset){
		auto& device = _context.graphics.internal.device;

		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(device.get(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{_buffer, offset, size};
	}

	void Buffer::writeToIndex(void *data, int index) {
		write(data, _instanceSize, index * _alignmentSize);
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

	VkBuffer Buffer::get() const{
		return _buffer;
	}

	VkDeviceMemory Buffer::memory() const{
		return _memory;
	}

	void* Buffer::mapped(){
		return _mapped;
	}
	
	uint32_t Buffer::instanceCount() const{
		return _instanceCount;
	}

	uint32_t Buffer::instanceSize() const{
		return _instanceSize;
	}

	void Buffer::update(VkCommandBuffer commandBuffer, void* data, VkDeviceSize size, VkDeviceSize offset){
		vkCmdUpdateBuffer(commandBuffer, _buffer, offset, size, data);
	}

	void Buffer::copy(VkCommandBuffer commandBuffer, VkBuffer destination, VkDeviceSize size, VkDeviceSize sourceOffset, VkDeviceSize destinationOffset){
		VkBufferCopy region;
		region.dstOffset = destinationOffset;
		region.srcOffset = sourceOffset;
		region.size = size;

		vkCmdCopyBuffer(commandBuffer, _buffer, destination, 1, &region);
	}
}