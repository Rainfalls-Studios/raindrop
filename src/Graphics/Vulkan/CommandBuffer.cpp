// #include <Raindrop/Graphics/CommandBuffer.hpp>
// #include <Raindrop/Graphics/Context.hpp>
// #include <Raindrop/Exceptions/VulkanExceptions.hpp>

// namespace Raindrop::Graphics{
// 	CommandBuffer::CommandBuffer() : 
// 		_context{nullptr},
// 		_commandBuffer{VK_NULL_HANDLE}
// 	{}

// 	CommandBuffer::CommandBuffer(Context& context, VkCommandBuffer commandBuffer) : 
// 		_context{&context},
// 		_commandBuffer{commandBuffer}
// 	{}

// 	CommandBuffer::~CommandBuffer(){}

// 	const VkCommandBuffer& CommandBuffer::get() const noexcept{
// 		return _commandBuffer;
// 	}

// 	void CommandBuffer::beginSingleUse(){
// 		VkCommandBufferBeginInfo info{
// 			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
// 			.pNext = nullptr,
// 			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
// 			.pInheritanceInfo = nullptr
// 		};

// 		Exceptions::checkVkOperation<VkCommandBuffer>(
// 			vkBeginCommandBuffer(_commandBuffer, &info),
// 			"Failed to begin command buffer",
// 			Exceptions::VulkanOperationType::BEGIN,
// 			_context->logger
// 		);
// 	}

// 	void CommandBuffer::end(){
// 		Exceptions::checkVkOperation<VkCommandBuffer>(
// 			vkEndCommandBuffer(_commandBuffer),
// 			"Failed to end command buffer",
// 			Exceptions::VulkanOperationType::END,
// 			_context->logger
// 		);
// 	}

// 	void CommandBuffer::submit(VkQueue queue, VkFence fence){
// 		VkSubmitInfo info{
// 			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
// 			.pNext = nullptr,
// 			.waitSemaphoreCount = 0,
// 			.pWaitSemaphores = nullptr,
// 			.pWaitDstStageMask = nullptr,
// 			.commandBufferCount = 1,
// 			.pCommandBuffers = &_commandBuffer,
// 			.signalSemaphoreCount = 0,
// 			.pSignalSemaphores = nullptr
// 		};

// 		Exceptions::checkVkOperation<VkQueue>(
// 			vkQueueSubmit(queue, 1, &info, fence),
// 			"Failed to submit command buffer",
// 			Exceptions::VulkanOperationType::SUBMIT,
// 			_context->logger
// 		);
// 	}

// }