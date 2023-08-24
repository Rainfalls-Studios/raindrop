#ifndef __RAINDROP_GRAPHICS_GRAPHICS_COMMAND_BUFFERS_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_COMMAND_BUFFERS_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GraphicsCommandBuffers{
		public:
			GraphicsCommandBuffers(GraphicsContext& context, uint32_t frameCount = 2);
			~GraphicsCommandBuffers();

			void updateFrameCount(uint32_t frameCount);
			VkCommandBuffer getCommandBuffer(uint32_t frame);

			std::weak_ptr<VkCommandBuffer> createSecondary();
			void freeSecondary(const std::weak_ptr<VkCommandBuffer>& commandBuffer);

		private:
			GraphicsContext& _context;

			std::vector<VkCommandBuffer> _mainCommandBuffers;
			std::list<std::shared_ptr<VkCommandBuffer>> _secondaryCommandBuffers;

			void freeMainCommandBuffers();
			void freeSecondaryCommandBuffer();
	};
}

#endif