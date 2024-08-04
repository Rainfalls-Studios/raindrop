#ifndef __RAINDROP_GRAPHICS_COMMAND_BUFFER_HPP__
#define __RAINDROP_GRAPHICS_COMMAND_BUFFER_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics{
	class CommandBuffer{
		public:
			CommandBuffer();
			CommandBuffer(Context& context, VkCommandBuffer commandBuffer);
			~CommandBuffer();

			const VkCommandBuffer& get() const noexcept;

			void beginSingleUse();

			void end();
			void submit(VkQueue queue, VkFence fence = VK_NULL_HANDLE);

		private:
			Context* _context;
			VkCommandBuffer _commandBuffer;
	};
}

#endif