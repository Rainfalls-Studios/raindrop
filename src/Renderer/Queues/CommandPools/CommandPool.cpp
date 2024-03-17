#include <Raindrop/Renderer/Queues/CommandPools/CommandPool.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Renderer::Queues{
	CommandPool::CommandPool(Context& context) : 
		_context{context},
		_pool{VK_NULL_HANDLE}
	{
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkCommandPoolCreateInfo info = createInfo();

		if (vkCreateCommandPool(device.get(), &info, allocationCallbacks, &_pool) != VK_SUCCESS){
			spdlog::error("Failed to create command pool");
			throw std::runtime_error("Failed to create command pool");
		}
	}

	CommandPool::~CommandPool(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_pool != VK_NULL_HANDLE){
			vkDestroyCommandPool(device.get(), _pool, allocationCallbacks);
			_pool = VK_NULL_HANDLE;
		}
	}

	VkCommandPool CommandPool::get() const{
		return _pool;
	}
}