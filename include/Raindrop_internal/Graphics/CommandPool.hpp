#ifndef __RAINDROP_INTERNAL_GRAPHICS_COMMAND_POOL_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_COMMAND_POOL_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct CommandPoolConfigInfo{
		VkCommandPoolCreateFlags flags;
		uint32_t familyIndex;
	};

	class CommandPool{
		public:
			CommandPool(Context& context, const CommandPoolConfigInfo& config);
			~CommandPool();

			VkCommandPool get() const;
			std::vector<VkCommandBuffer> allocate(const uint32_t& count, const VkCommandBufferLevel& level);
			void free(const std::vector<VkCommandBuffer>& commandBuffer);

			void reset(const VkCommandPoolResetFlags& flags);

		private:
			Context& _context;
			VkCommandPool _commandPool;
	};

	class CommandPoolHandler{
		public:
			// CommandPoolHnadler(Context& context);

		private:

	};
}

#endif