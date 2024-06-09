#include <Raindrop_internal/Graphics/Buffer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Utils/alignement.hpp>

namespace Raindrop::Internal::Graphics{
	Buffer::Buffer(Context& context) :
		_context{context},
		_memory{VK_NULL_HANDLE},
		_buffer{VK_NULL_HANDLE},
		_mapped{nullptr},
		_size{0},
		_minAllignement{0},
		_usageFlags{0},
		_memoryPropertyFlags{0}
	{}

	Buffer::~Buffer(){
		unmap();
		free();
	}

	void Buffer::allocate(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkBufferCreateFlags createFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize minOffsetAlignment){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		free();
		
		_size = static_cast<std::size_t>(Utils::alignTo(size, minOffsetAlignment));
		_usageFlags = usageFlags;
		_createFlags = createFlags;
		_memoryPropertyFlags = memoryPropertyFlags;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<uint32_t>(_size);
		bufferInfo.usage = usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.flags = createFlags;

		if (vkCreateBuffer(device.get(), &bufferInfo, allocationCallbacks, &_buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.get(), _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, memoryPropertyFlags);

		if (vkAllocateMemory(device.get(), &allocInfo, allocationCallbacks, &_memory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(device.get(), _buffer, _memory, 0);
	}

	void Buffer::free(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_buffer) vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
		if (_memory) vkFreeMemory(device.get(), _memory, allocationCallbacks);
	}

	VkResult Buffer::map(VkDeviceSize size, VkDeviceSize offset){
		return vkMapMemory(_context.getDevice().get(), _memory, offset, size, 0, &_mapped);
	}

	void Buffer::unmap(){
		if (_mapped){
			vkUnmapMemory(_context.getDevice().get(), _memory);
			_mapped = nullptr;
		}
	}

	void Buffer::write(void *data, VkDeviceSize size, VkDeviceSize offset) {
		if (size == VK_WHOLE_SIZE) {
			memcpy(_mapped, data, _size);
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
		return vkFlushMappedMemoryRanges(_context.getDevice().get(), 1, &mappedRange);
	}

	VkResult Buffer::invalidate(VkDeviceSize size, VkDeviceSize offset) {
		VkMappedMemoryRange mappedRange = {};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		return vkInvalidateMappedMemoryRanges(_context.getDevice().get(), 1, &mappedRange);
	}

	VkDescriptorBufferInfo Buffer::descriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
		return VkDescriptorBufferInfo{_buffer, offset, size};
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

	const std::size_t& Buffer::getSize() const noexcept{
		return _size;
	}

	const std::size_t& Buffer::getMinAllignement() const noexcept{
		return _minAllignement;
	}

	const VkBufferUsageFlags& Buffer::getUsageFlags() const noexcept{
		return _usageFlags;
	}

	const VkBufferCreateFlags& Buffer::getCreateFlags() const noexcept{
		return _createFlags;
	}

	const VkMemoryPropertyFlags& Buffer::getMemoryPropertyFlags() const noexcept{
		return _memoryPropertyFlags;
	}
}