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

			Context& getContext();

		private:
			struct EventCache;
			EventCache* _eventCache;

			Internal::Context& _internal;
			Context* _context;

			void registerEvents();
			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void unregisterEvents();
			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			VkCommandBuffer beginFrame();
			void endFrame();

			void updateSwapchainSize();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;

			std::shared_ptr<ShaderLoader> _shaderLoader;
	};
}

#endif