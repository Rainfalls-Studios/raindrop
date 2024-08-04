#ifndef __RAINDROP_GRAPHICS_COMMAND_POOL_HPP__
#define __RAINDROP_GRAPHICS_COMMAND_POOL_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class CommandPool{
		public:
			CommandPool();
			~CommandPool();

			CommandPool& prepare(Context& context);
			void initialize();
			void release();

			CommandPool& setCreateFlags(const VkCommandPoolCreateFlags& flags);
			CommandPool& setQueueFamily(const std::uint32_t& index);

			const VkCommandPool& get() const noexcept;

			std::vector<CommandBuffer> allocate(const std::uint32_t& count, const VkCommandBufferLevel& level);
			void free(const std::vector<CommandBuffer>& commandBuffers);
			
			void reset(const VkCommandPoolResetFlags& flags);
			void trim(const VkCommandPoolTrimFlags& flags);

		private:
			Context* _context;
			VkCommandPool _pool;

			VkCommandPoolCreateFlags _flags;
			std::uint32_t _familyIndex;
	};
}

#endif