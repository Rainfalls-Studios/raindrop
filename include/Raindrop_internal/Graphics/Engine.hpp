#ifndef __RAINDROP_INTERNAL_GRAPHICS_ENGINE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_ENGINE_HPP__

#include "common.hpp"
#include <Raindrop_internal/Events/common.hpp>

namespace Raindrop::Internal::Graphics{
	class Engine{
		public:
			Engine(Internal::Context& internal);
			~Engine();
			
			void render();
			void events();

			Context& getContext() noexcept;
			const Context& getContext() const noexcept;

		private:
			struct EventCache;
			EventCache* _eventCache;

			Internal::Context& _internal;
			Context* _context;

			void registerEvents();
			void createFrameCommandPool();
			void allocateFrameCommandBuffers();

			void unregisterEvents();
			void freeFrameCommandBuffers();
			void destroyFrameCommandPool();

			VkCommandBuffer beginFrame();
			void endFrame();

			void updateSwapchainSize();

			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;

			std::shared_ptr<ShaderLoader> _shaderLoader;

			Events::EventID _OnFrame;
			Events::EventID _OnSwapchainPass;
	};
}

#endif