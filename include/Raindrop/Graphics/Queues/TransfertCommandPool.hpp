#ifndef __RAINDROP_GRAPHICS_QUEUES_TRANSFERT_COMMAND_POOL_HPP__
#define __RAINDROP_GRAPHICS_QUEUES_TRANSFERT_COMMAND_POOL_HPP__

#include <Raindrop/Graphics/Queues/common.hpp>

namespace Raindrop::Graphics::Queues{
	class TransfertCommandPool{
		public:
			TransfertCommandPool(GraphicsContext& context);
			~TransfertCommandPool();

			VkCommandPool primary() const;
			VkCommandPool singleTime() const;

			VkCommandBuffer beginSingleTime();
			void endSingleTime(VkCommandBuffer commandBuffer);

		private:
			GraphicsContext& _context;
			VkCommandPool _primaryCommandPool = VK_NULL_HANDLE;
			VkCommandPool _singleTimeCommandPool = VK_NULL_HANDLE;
	};
}

#endif