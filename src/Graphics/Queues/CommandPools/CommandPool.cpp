#include <Raindrop/Graphics/Queues/CommandPools/CommandPool.hpp>
#include <Raindrop/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Queues{
	CommandPool::CommandPool(Context& context) : 
		_context{context},
		_pool{VK_NULL_HANDLE}
	{}

	CommandPool::~CommandPool(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_pool != VK_NULL_HANDLE){
			vkDestroyCommandPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}
	}

	void CommandPool::create(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

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