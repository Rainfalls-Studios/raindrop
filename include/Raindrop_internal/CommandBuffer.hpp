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

	VkCommandPoolCreateFlags toVulkan(const CommandBuffer::Pool::Flags& flags);
	VkQueueFlags toVulkan(const CommandBuffer::Usage& usage);
	VkCommandBufferUsageFlags toVulkan(const CommandBuffer::Flags& flags);
}

#endif