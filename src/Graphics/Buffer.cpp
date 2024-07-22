#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Utils/alignement.hpp>

namespace Raindrop::Graphics{
	Buffer::BuildInfo::BuildInfo() : 
		size{0},
		alignement{1},
		usage{0},
		createFlags{0},
		memoryPropertiess{0},
		queueFamilies{},
		sharingMode{VK_SHARING_MODE_EXCLUSIVE}
	{}

	Buffer::Buffer() noexcept : 
		_context{nullptr},
		_buffer{VK_NULL_HANDLE},
		_memory{VK_NULL_HANDLE},
		_mapped{nullptr},
		_buildInfo{},
		_size{0}
	{}
	
	Buffer::~Buffer(){
		release();
	}


	Buffer::Buffer(Buffer&& other) :
		_context{nullptr},
		_buffer{VK_NULL_HANDLE},
		_memory{VK_NULL_HANDLE},
		_mapped{nullptr},
		_buildInfo{},
		_size{0}
	{
		swap(*this, other);
	}
	
	Buffer& Buffer::operator=(Buffer&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Buffer& A, Buffer& B){
		std::swap(A._context, B._context);
		std::swap(A._buffer, B._buffer);
		std::swap(A._memory, B._memory);
		std::swap(A._mapped, B._mapped);
		std::swap(A._buildInfo, B._buildInfo);
		std::swap(A._size, B._size);
	}

	void Buffer::prepare(Context& context){
		_context = &context;
		_buildInfo = std::make_unique<BuildInfo>();
	}

	void Buffer::release(){
		_buildInfo.reset();

		if (!_context) return;
		free();
		_context = nullptr;
	}

	void Buffer::allocate(){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;
		
		_size = static_cast<std::size_t>(Utils::alignTo(_buildInfo->size, _buildInfo->alignement));

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = static_cast<VkDeviceSize>(_size);
		bufferInfo.usage = _buildInfo->usage;
		bufferInfo.flags = _buildInfo->createFlags;
		bufferInfo.sharingMode = _buildInfo->sharingMode;
		bufferInfo.queueFamilyIndexCount = static_cast<uint32_t>(_buildInfo->queueFamilies.size());
		bufferInfo.pQueueFamilyIndices = _buildInfo->queueFamilies.data();

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
		allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, _buildInfo->memoryPropertiess);

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

		_buildInfo.reset();
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

	Buffer& Buffer::setSize(const std::size_t& size){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->size = size;
		return *this;
	}

	Buffer& Buffer::setAlignement(const std::size_t& alignement){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->alignement = alignement;
		return *this;
	}

	Buffer& Buffer::setUsage(const VkBufferUsageFlags& usage){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->usage = usage;
		return *this;
	}

	Buffer& Buffer::setCreateFlags(const VkBufferCreateFlags& flags){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->createFlags = flags;
		return *this;
	}

	Buffer& Buffer::setMemoryProperties(const VkMemoryPropertyFlags& properties){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->memoryPropertiess = properties;
		return *this;
	}

	Buffer& Buffer::setQueueFamilies(const std::vector<uint32_t>& families){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->queueFamilies = families;
		return *this;
	}

	Buffer& Buffer::setSharingMode(const VkSharingMode& sharingMode){
		if (!_buildInfo) throw std::runtime_error("The buffer has already been allocated");
		_buildInfo->sharingMode = sharingMode;
		return *this;
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