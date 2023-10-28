#ifndef __RAINDROP_GRAPHICS_INTERNAL_COMMAND_BUFFERS_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_COMMAND_BUFFERS_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class CommandBuffer{
		public:
			CommandBuffer(CommandPool& pool, VkCommandBuffer commandBuffer, VkCommandBufferLevel level);
			~CommandBuffer();

			bool operator==(const CommandBuffer& other) const;

			VkCommandBuffer get() const;

			void begin(VkCommandBufferUsageFlags usage = 0);
			void end();

			bool recording();
			
		private:
			CommandPool& _pool;
			VkCommandBuffer _commandBuffer;
			VkCommandBufferLevel _level;

			bool _recording;
	};
}

#endif