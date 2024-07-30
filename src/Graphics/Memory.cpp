#include <Raindrop/Graphics/Memory.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Memory::Memory() noexcept : 
		_context{nullptr},
		_memory{VK_NULL_HANDLE},
		_info{}
	{}

	Memory::~Memory(){
		release();
	}

	Memory::Memory(Memory&& other) : 
		_context{nullptr},
		_memory{VK_NULL_HANDLE},
		_info{}
	{
		swap(*this, other);
	}

	Memory& Memory::operator=(Memory&& other){
		swap(*this, other);
		return *this;
	}

	void swap(Memory& A, Memory& B){
		std::swap(A._context, B._context);
		std::swap(A._memory, B._memory);
		std::swap(A._info, B._info);
	}

	void Memory::prepare(Context& context){
		_context = &context;
		_info = std::make_unique<BuildInfo>();
	}

	void Memory::initialize(){
		if (!_context){
			spdlog::warn("Attempt to initialized a non prepared memory");
			throw std::runtime_error("The memory has not been prepared !");
		}

		if (!_info){
			_context->logger->warn("Attempt to initialized a non prepared memory");
			throw std::runtime_error("The memory has not been prepared !");
		}

		VkMemoryAllocateInfo info{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = static_cast<VkDeviceSize>(_info->size),
			.memoryTypeIndex = static_cast<uint32_t>(_info->memoryIndex)
		};

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkDeviceMemory>(
			vkAllocateMemory(device.get(), &info, allocationCallbacks, &_memory),
			"Failed to allocate memory",
			_context->logger
		);
	}

	void Memory::release(){
		if (!_context) return;

		auto& device = _context->getDevice();
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_memory){
			_context->logger->trace("Freeing device memory...");

			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}

		_context = nullptr;
		_info.reset();
	}

	Memory& Memory::setSize(const std::size_t& size){
		getInfo().size = size;
		return *this;
	}

	Memory& Memory::setMemoryIndex(const std::uint32_t& index){
		getInfo().memoryIndex = index;
		return *this;
	}

	const VkDeviceMemory& Memory::get() const noexcept{
		return _memory;
	}

	Memory::BuildInfo& Memory::getInfo(){
		if (!_context){
			spdlog::warn("Attempt to access change build info of a non prepared memory");
			throw std::runtime_error("The memory has not been prepared !");
		}

		if (!_info){
			_context->logger->warn("Attempt to access change build info of a non prepared memory");
			throw std::runtime_error("The memory has not been prepared !");
		}

		return *_info;
	}

	void Memory::map(const std::size_t& size, const std::size_t& offset){
		VkDeviceSize deviceSize = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkMapMemory(_context->getDevice().get(), _memory, static_cast<VkDeviceSize>(offset), deviceSize, 0, &_mapped),
			"Failed to map memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}

	void Memory::unmap(){
		if (_mapped){
			vkUnmapMemory(_context->getDevice().get(), _memory);
			_mapped = nullptr;
		}
	}

	void* Memory::getMappedPointer(){
		return _mapped;
	}

	void Memory::flush(const std::size_t& size, const std::size_t& offset){
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = static_cast<VkDeviceSize>(offset);
		mappedRange.size = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkFlushMappedMemoryRanges(_context->getDevice().get(), 1, &mappedRange),
			"Failed to flush memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}

	void Memory::invalidate(const std::size_t& size, const std::size_t& offset){
		VkMappedMemoryRange mappedRange{};
		mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedRange.memory = _memory;
		mappedRange.offset = static_cast<VkDeviceSize>(offset);
		mappedRange.size = size == WHOLE_SIZE ? VK_WHOLE_SIZE : static_cast<VkDeviceSize>(size);

		Exceptions::checkVkOperation<VkBuffer>(
			vkFlushMappedMemoryRanges(_context->getDevice().get(), 1, &mappedRange),
			"Failed to flush memory",
			Exceptions::VulkanOperationType::MAPPING,
			_context->logger
		);
	}
}