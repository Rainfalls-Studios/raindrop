#ifndef __RAINDROP_INTERNAL_COMMAND_BUFFER_HPP__
#define __RAINDROP_INTERNAL_COMMAND_BUFFER_HPP__

#include <Raindrop/CommandBuffer.hpp>
#include <Raindrop_internal/Graphics/common.hpp>
#include <Raindrop_internal/Graphics/CommandPool.hpp>

namespace Raindrop{
	struct CommandBuffer::Pool::Impl{
		Internal::Context* context;
		std::shared_ptr<Internal::Graphics::CommandPool> commandPool;

		Impl() : 
			context{nullptr},
			commandPool{nullptr}
		{}
	};

	struct CommandBuffer::Impl{
		Internal::Context* context;
		VkCommandBuffer commandBuffer;

		Impl() : 
			context{nullptr},
			commandBuffer{nullptr}
		{}
	};

	VkCommandPoolCreateFlags toVulkan(const CommandBuffer::Pool::Flags& flags);
	VkQueueFlags toVulkan(const CommandBuffer::Usage& usage);
	VkCommandBufferUsageFlags toVulkan(const CommandBuffer::Flags& flags);
}

#endif