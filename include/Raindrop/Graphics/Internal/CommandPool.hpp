#ifndef __RAINDROP_GRAPHICS_INTERNAL_COMMAND_POOL_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_COMMAND_POOL_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class CommandPool{
		friend class CommandBuffer;
		public:
			CommandPool(Context& context, VkCommandPool commandPool, uint32_t familyIndex, VkCommandPoolCreateFlags flags);
			~CommandPool();

			bool isTransient() const;
			bool isResetable() const;
			bool isProtected() const;

			uint32_t familyIndex() const;

			void reset(VkCommandPoolResetFlags flags);
			void trim(VkCommandPoolTrimFlags flags = 0);

			CommandBuffer& allocate(VkCommandBufferLevel level);
			std::vector<CommandBuffer&> allocate(VkCommandBufferLevel level, std::size_t count);

			void free(CommandBuffer& commandBuffer);
			void free(const std::vector<CommandBuffer&>& commandBuffers);

		private:
			Context& _context;

			std::list<std::unique_ptr<CommandBuffer>> _commandBuffers;

			VkCommandPool _commandPool;
			uint32_t _familyIndex;
			VkCommandPoolCreateFlags _flags;

	};
}

#endif