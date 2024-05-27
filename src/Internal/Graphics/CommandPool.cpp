#include <Raindrop_internal/Graphics/CommandPool.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	CommandPool::CommandPool(Context& context, const CommandPoolConfigInfo& config) :
		_context{context},
		_commandPool{VK_NULL_HANDLE}{

		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		VkCommandPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = config.flags;
		info.queueFamilyIndex = config.familyIndex;

		Exceptions::checkVkCreation<VkCommandPool>(
			vkCreateCommandPool(device.get(), &info, allocationCallbacks, &_commandPool),
			"Failed to create command pool",
			_context.getLogger()
		);
	}

	CommandPool::~CommandPool(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_commandPool != VK_NULL_HANDLE){
			vkDestroyCommandPool(device.get(), _commandPool, allocationCallbacks);
			_commandPool = VK_NULL_HANDLE;
		}
	}

	VkCommandPool CommandPool::get() const{
		return _commandPool;
	}


	std::vector<VkCommandBuffer> CommandPool::allocate(const uint32_t& count, const VkCommandBufferLevel& level){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		std::vector<VkCommandBuffer> out(count);

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = count;
		info.commandPool = _commandPool;
		info.level = level;


		Exceptions::checkVkOperation<VkCommandPool>(
			vkAllocateCommandBuffers(device.get(), &info, out.data()),
			"Failed to allocate command buffers",
			Exceptions::VulkanOperationType::ALLOCATION,
			_context.getLogger()
		);
	}

	void CommandPool::free(const std::vector<VkCommandBuffer>& commandBuffers){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		vkFreeCommandBuffers(
			device.get(),
			_commandPool,
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data()
		);
	}


	void CommandPool::reset(const VkCommandPoolResetFlags& flags){
		auto& device = _context.getDevice();

		Exceptions::checkVkOperation<VkCommandPool>(
			vkResetCommandPool(device.get(), _commandPool, flags),
			"Failed to reset command pool",
			Exceptions::VulkanOperationType::RESET,
			_context.getLogger()
		);
	}
}