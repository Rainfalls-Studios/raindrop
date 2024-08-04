#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Utils/alignement.hpp>
#include <Raindrop/Graphics/Memory.hpp>

namespace Raindrop::Graphics{
	Buffer::BuildInfo::BuildInfo() : 
		size{0},
		alignement{1},
		usage{0},
		createFlags{0},
		memoryPropertiess{0},
		queueFamilies{},
		sharingMode{VK_SHARING_MODE_EXCLUSIVE},
		memory{},
		memoryOffset{0}
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

	Buffer& Buffer::prepare(Context& context){
		_context = &context;
		_buildInfo = std::make_unique<BuildInfo>();
		return *this;
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

		if (!_buildInfo->memory){
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device.get(), _buffer, &memRequirements);

			_memory = std::make_shared<Memory>();
			_memory->prepare(*_context);
			_memory->setMemoryIndex(device.findMemoryType(memRequirements.memoryTypeBits, _buildInfo->memoryPropertiess));
			_memory->setSize(memRequirements.size);

			_memory->initialize();
		} else {
			_memory = _buildInfo->memory;
		}
		
		_context->logger->trace("Binding buffer and memory...");

		Exceptions::checkVkOperation<VkDeviceMemory>(
			vkBindBufferMemory(device.get(), _buffer, _memory->get(), static_cast<VkDeviceSize>(_buildInfo->memoryOffset)),
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
		
		_memory.reset();

		if (_buffer != VK_NULL_HANDLE){
			vkDestroyBuffer(device.get(), _buffer, allocationCallbacks);
			_buffer = VK_NULL_HANDLE;
		}

		_mapped = nullptr;
	}

	Buffer& Buffer::setSize(const std::size_t& size){
		getInfo().size = size;
		return *this;
	}

	Buffer& Buffer::setAlignement(const std::size_t& alignement){
		getInfo().alignement = alignement;
		return *this;
	}

	Buffer& Buffer::setUsage(const VkBufferUsageFlags& usage){
		getInfo().usage = usage;
		return *this;
	}

	Buffer& Buffer::setCreateFlags(const VkBufferCreateFlags& flags){
		getInfo().createFlags = flags;
		return *this;
	}

	Buffer& Buffer::setMemoryProperties(const VkMemoryPropertyFlags& properties){
		getInfo().memoryPropertiess = properties;
		return *this;
	}

	Buffer& Buffer::setQueueFamilies(const std::vector<uint32_t>& families){
		getInfo().queueFamilies = families;
		return *this;
	}

	Buffer& Buffer::setSharingMode(const VkSharingMode& sharingMode){
		_buildInfo->sharingMode = sharingMode;
		return *this;
	}

	Buffer& Buffer::setMemory(const std::shared_ptr<Memory>& memory){
		getInfo().memory = memory;
		return *this;
	}

	Buffer& Buffer::setMemoryOffset(const std::size_t& offset){
		getInfo().memoryOffset = offset;
		return *this;
	}

	const VkBuffer& Buffer::get() const noexcept{
		return _buffer;
	}

	const std::shared_ptr<Memory>& Buffer::getMemory() const noexcept{
		return _memory;
	}

	void Buffer::map(const std::size_t& size, const std::size_t& offset){
		_memory->map(size, offset);
	}

	void Buffer::unmap(){
		_memory->unmap();
	}

	bool Buffer::isMapped() const noexcept{
		return _memory->getMappedPointer() != nullptr;
	}

	void* Buffer::getMappedPointer() noexcept{
		return _memory->getMappedPointer();
	}

	void Buffer::flush(const std::size_t& size, const std::size_t& offset){
		_memory->flush(size, offset);
	}

	void Buffer::invalidate(const std::size_t& size, const std::size_t& offset){
		_memory->invalidate(size, offset);
	}

	Buffer::BuildInfo& Buffer::getInfo(){
		if (!_context){
			spdlog::warn("Attempt to access change build info of a non prepared buffer");
			throw std::runtime_error("The buffer has not been prepared !");
		}

		if (!_buildInfo){
			_context->logger->warn("Attempt to access change build info of a non prepared buffer");
			throw std::runtime_error("The buffer has not been prepared !");
		}

		return *_buildInfo;
	}
}