#include <Raindrop_internal/Graphics/Memory.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	Memory::Memory(Context& context, MemoryType& type, const std::size_t& size) : 
			_context{context},
			_memory{VK_NULL_HANDLE},
			_size{0},
			_type{type}
		{

		auto& device = context.getDevice();
		auto& allocationCallbacks = context.getAllocationCallbacks();

		VkMemoryAllocateInfo info{};
		info.sType =VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		info.memoryTypeIndex = type.getIndex();
		info.allocationSize = static_cast<uint32_t>(size);

		Exceptions::checkVkCreation<VkDeviceMemory>(
			vkAllocateMemory(device.get(), &info, allocationCallbacks, &_memory),
			"Failed to allocate device memory",
			context.getLogger()
		);

		_size = size;
		type.notifyAllocation(size);
	}

	Memory::Memory(Context& context, MemoryType& type) : 
		_context{context},
		_memory{VK_NULL_HANDLE},
		_size{0},
		_type{type}
	{}

	Memory::~Memory(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		vkFreeMemory(device.get(), _memory, allocationCallbacks);

		_type.notifyFree(_size);
	}

	Memory::Memory(Memory&& other) noexcept :
		_context{other._context},
		_memory{other._memory},
		_size{other._size},
		_type{other._type}
	{}

	VkDeviceMemory Memory::get() const noexcept{
		return _memory;
	}

	std::size_t Memory::getSize() const noexcept{
		return _size;
	}

	const MemoryType& Memory::getType() const noexcept{
		return _type;
	}

	MemoryType::MemoryType(const std::size_t& index, const VkMemoryType& info) : 
		_index{index},
		_flags{info.propertyFlags}
	{}

	MemoryType::~MemoryType(){}

	const std::size_t& MemoryType::getIndex() const noexcept{
		return _index;
	}

	const VkMemoryPropertyFlags& MemoryType::getFlags() const noexcept{
		return _flags;
	}

	// Could be used for debug checks, ...
	void MemoryType::notifyAllocation(const std::size_t& size) noexcept{}
	void MemoryType::notifyFree(const std::size_t& size) noexcept{}



	MemoryTypes::MemoryTypes(Context& context) : _context{context}{
		auto& memoryProps = _context.getPhysicalDevice().memoryProperties();

		_types.reserve(static_cast<std::size_t>(memoryProps.memoryTypeCount));

		for (uint32_t i=0; i<memoryProps.memoryTypeCount; i++){
			_types.emplace_back(MemoryType(static_cast<std::size_t>(i), memoryProps.memoryTypes[i]));
		}
	}

	MemoryTypes::~MemoryTypes(){
		_types.clear();
	}

	const std::vector<MemoryType>& MemoryTypes::getTypes() const noexcept{
		return _types;
	}

	std::vector<MemoryType>& MemoryTypes::getTypes() noexcept{
		return _types;
	}

	MemoryType& MemoryTypes::findBest(const VkMemoryPropertyFlags& required){

		MemoryType* bestCandidat = nullptr; 
		int bestScore = INT_MIN;

		for (auto& type : _types){
			const auto& flags = type.getFlags();

			const int score = (flags ^ required) * (2 * int((flags & required) == required) - 1);

			if (score > bestScore){
				bestScore = score;
				bestCandidat = &type;
			}
		}

		if (bestCandidat == nullptr){
			_context.getLogger()->error("Could not find a memory type that supportes required flags");
			throw std::runtime_error("Could not fullfill requirements");
		}

		return *bestCandidat;
	}


	MemoryType& MemoryTypes::get(const std::size_t& index){
		return _types.at(index);
	}

	const MemoryType& MemoryTypes::get(const std::size_t& index) const{
		return _types.at(index);
	}
}