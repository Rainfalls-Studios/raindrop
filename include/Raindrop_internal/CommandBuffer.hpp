#ifndef __RAINDROP_INTERNAL_COMMAND_BUFFER_HPP__
#define __RAINDROP_INTERNAL_COMMAND_BUFFER_HPP__

#include <Raindrop/CommandBuffer.hpp>
#include <Raindrop_internal/Graphics/common.hpp>
#include <Raindrop_internal/Graphics/CommandPool.hpp>

namespace Raindrop{
	struct CommandBuffer::Pool::Impl{
		std::shared_ptr<Internal::Graphics::CommandPool> commandPool;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	struct CommandBuffer::Impl{
		VkCommandBuffer commandBuffer;
		Context* context;

		Impl(Context& context) : context{&context}{}
	};

	VkCommandPoolCreateFlags toVulkan(const CommandBuffer::Pool::Flags& flags){
		VkCommandPoolCreateFlags out = 0;

		if (flags.has(CommandBuffer::Pool::Flags::SHORT_LIVED)) out |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		if (flags.has(CommandBuffer::Pool::Flags::INDEPENDENT_RESET)) out |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		if (flags.has(CommandBuffer::Pool::Flags::PROTECTED)) out |= VK_COMMAND_POOL_CREATE_PROTECTED_BIT;

		return out;
	}

	VkQueueFlags toVulkan(const CommandBuffer::Usage& usage){
		VkQueueFlags out = 0;

		if (usage.has(CommandBuffer::Usage::GRAPHICS)) out |= VK_QUEUE_GRAPHICS_BIT;
		if (usage.has(CommandBuffer::Usage::COMPUTE)) out |= VK_QUEUE_COMPUTE_BIT;
		if (usage.has(CommandBuffer::Usage::TRANSFER)) out |= VK_QUEUE_TRANSFER_BIT;
		if (usage.has(CommandBuffer::Usage::SPARSE_BINDING)) out |= VK_QUEUE_SPARSE_BINDING_BIT;
		if (usage.has(CommandBuffer::Usage::PROTECTED)) out |= VK_QUEUE_PROTECTED_BIT;

		// VK_KHR_video_decode_queue
		if (usage.has(CommandBuffer::Usage::VIDEO_DECODE)) out |= VK_QUEUE_VIDEO_DECODE_BIT_KHR;

		// VK_KHR_video_encode_queue
		if (usage.has(CommandBuffer::Usage::VIDEO_ENCODE)) out |= VK_QUEUE_VIDEO_ENCODE_BIT_KHR;

		// VK_NV_optical_flow
		if (usage.has(CommandBuffer::Usage::OPTICAL_FLOW)) out |= VK_QUEUE_OPTICAL_FLOW_BIT_NV;

		return out;
	}

	VkCommandBufferUsageFlags toVulkan(const CommandBuffer::Flags& flags){
		VkCommandBufferUsageFlags out = 0;

		if (flags.has(CommandBuffer::Flags::SINGLE_SUBMIT)) out |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (flags.has(CommandBuffer::Flags::RENDER_PASS_CONTINUE)) out |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		if (flags.has(CommandBuffer::Flags::SIMULTANEOUS_USE)) out |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		return out;
	}
}

#endif