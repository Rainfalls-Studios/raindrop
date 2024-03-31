#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include "common.hpp"

namespace Raindrop::Graphics{
	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			void render();
			void events();

			Context& context();

		private:
			Context* _context;

			VkCommandBuffer beginFrame();
			void endFrame();

			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;
	};
}

#endif