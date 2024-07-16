#include <Raindrop/Graphics/CommandPool.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	CommandPool::CommandPool() : 
		_context{nullptr},
		_pool{VK_NULL_HANDLE}
	{}

	CommandPool::~CommandPool(){
		release();
	}

	void CommandPool::prepare(Context& context){
		_context = &context;
	}

	void CommandPool::initialize(){
		_context->logger->info("Creating command pool...");


		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = _flags;
		info.queueFamilyIndex = static_cast<uint32_t>(_familyIndex);

		auto& device = _context->core.device;
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		Exceptions::checkVkCreation<VkCommandPool>(
			vkCreateCommandPool(device.get(), &info, allocationCallbacks, &_pool),
			"Failed to create command pool",
			_context->logger
		);
	}

	void CommandPool::release(){
		if (!_context) return;

		auto& device = _context->core.device;
		auto& allocationCallbacks = _context->core.allocationCallbacks;

		if (_pool != VK_NULL_HANDLE){
			_context->logger->info("Destroying command pool...");
			vkDestroyCommandPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}

		_context = nullptr;
	}

	const VkCommandPool& CommandPool::get() const noexcept{
		return _pool;
	}

	CommandPool& CommandPool::setCreateFlags(const VkCommandPoolCreateFlags& flags){
		if (_pool != VK_NULL_HANDLE) throw std::runtime_error("Cannot change build properties when the pool has already been initialied");
		_flags = flags;
		return *this;
	}

	CommandPool& CommandPool::setQueueFamily(const std::uint32_t& index){
		if (_pool != VK_NULL_HANDLE) throw std::runtime_error("Cannot change build properties when the pool has already been initialied");
		_familyIndex = index;
		return *this;
	}

}