#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDERER_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	class Renderer{
		public:
			Renderer(Internal::Context& internal);
			~Renderer();
			
			void render();
			void events();

		private:
			struct EventCache;
			EventCache* _eventCache;

			Internal::Context& _internal;
			Context* _context;

			void registerEvents();
			void unregisterEvents();

			VkCommandBuffer beginFrame();
			void endFrame();

			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			void updateSwapchainSize();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;
	};
}

#endif