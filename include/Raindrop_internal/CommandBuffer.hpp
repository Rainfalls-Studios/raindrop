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
}

#endif