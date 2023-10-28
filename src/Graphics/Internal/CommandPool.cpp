#include <Raindrop/Graphics/Internal/CommandPool.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/CommandBuffer.hpp>

namespace Raindrop::Graphics::Internal{
	CommandPool::CommandPool(Context& context, VkCommandPool commandPool, uint32_t familyIndex, VkCommandPoolCreateFlags flags) : 
		_context{context},
		_commandPool{commandPool},
		_familyIndex{familyIndex},
		_flags{flags}
	{}

	CommandPool::~CommandPool(){
		auto device = _context.device.get();
		auto& allocationCallbacks = _context.graphics.allocationCallbacks;

		if (_commandPool) vkDestroyCommandPool(device, _commandPool, allocationCallbacks);
		_commandPool = VK_NULL_HANDLE;
	}

	bool CommandPool::isTransient() const{
		return _flags & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	}

	bool CommandPool::isResetable() const{
		return _flags & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	}

	bool CommandPool::isProtected() const{
		return _flags & VK_COMMAND_POOL_CREATE_PROTECTED_BIT;
	}

	uint32_t CommandPool::familyIndex() const{
		return _familyIndex;
	}

	void CommandPool::reset(VkCommandPoolResetFlags flags){
		assert(isResetable());

		auto device = _context.device.get();

		if (vkResetCommandPool(device, _commandPool, flags) != VK_SUCCESS){
			_context.logger.error("Failed to reset command pool");
			throw std::runtime_error("Failed to reset command pool");
		}
	}

	void CommandPool::trim(VkCommandPoolTrimFlags flags){
		auto device = _context.device.get();
		vkTrimCommandPool(device, _commandPool, flags);
	}

	CommandBuffer& CommandPool::allocate(VkCommandBufferLevel level){
		auto device = _context.device.get();

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = 1;
		info.commandPool = _commandPool;
		info.level = level;

		VkCommandBuffer commandBuffer;
		if (vkAllocateCommandBuffers(device, &info, &commandBuffer) != VK_SUCCESS){
			_context.logger.error("Failed to allocate a command buffer");
			throw std::runtime_error("Failed to allocate command buffer");
		}

		std::unique_ptr<CommandBuffer> cmdBuffer = std::make_unique<CommandBuffer>(*this, commandBuffer, level);
		_commandBuffers.push_back(std::move(cmdBuffer));
		return *_commandBuffers.back();
	}

	std::vector<std::reference_wrapper<CommandBuffer>> CommandPool::allocate(VkCommandBufferLevel level, std::size_t count){
		auto device = _context.device.get();

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandBufferCount = static_cast<uint32_t>(count);
		info.commandPool = _commandPool;
		info.level = level;

		std::vector<VkCommandBuffer> commandBuffers(count);
		if (vkAllocateCommandBuffers(device, &info, commandBuffers.data()) != VK_SUCCESS){
			_context.logger.error("Failed to allocate %d command buffers", count);
			throw std::runtime_error("Failed to allocate command buffers");
		}

		std::vector<std::reference_wrapper<CommandBuffer>> cmdBuffers;

		for (std::size_t i=0; i<count; i++){
			std::unique_ptr<CommandBuffer> cmdBuffer = std::make_unique<CommandBuffer>(*this, commandBuffers[i], level);
			_commandBuffers.push_back(std::move(cmdBuffer));
			cmdBuffers.emplace_back(std::ref(*_commandBuffers.back()));
		}

		return cmdBuffers;
	}

	void CommandPool::free(CommandBuffer& commandBuffer){
		_commandBuffers.remove_if(
			[&commandBuffer](const std::unique_ptr<CommandBuffer>& cmd){
				return commandBuffer == *cmd;
			}
		);
		
		auto device = _context.device.get();
		VkCommandBuffer cmdBuffer = commandBuffer.get();

		vkFreeCommandBuffers(device, _commandPool, 1, &cmdBuffer);
	}

	void CommandPool::free(const std::vector<std::reference_wrapper<CommandBuffer>>& commandBuffers){
		for (std::size_t i=0; i<commandBuffers.size(); i++){
			free(commandBuffers[i]);
		}
	}
}