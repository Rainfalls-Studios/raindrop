#include <Raindrop_internal/Graphics/Queues/CommandPools/CommandPool.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics::Queues{
	CommandPool::CommandPool(Context& context) : 
		_context{context},
		_pool{VK_NULL_HANDLE}
	{}

	CommandPool::~CommandPool(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_pool != VK_NULL_HANDLE){
			vkDestroyCommandPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}
	}

	void CommandPool::create(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		VkCommandPoolCreateInfo info = createInfo();

		Exceptions::checkVkCreation<VkCommandPool>(
			vkCreateCommandPool(device.get(), &info, allocationCallbacks, &_pool),
			"Failed to create command pool"
		);
	}

	VkCommandPool CommandPool::get() const{
		return _pool;
	}
}