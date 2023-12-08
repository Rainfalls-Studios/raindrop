#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_CONTEXT_HPP__

#include <Raindrop/Graphics/Framebuffer/common.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Framebuffer/FramebufferManager.hpp>
#include <Raindrop/Graphics/Framebuffer/Loader.hpp>

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Framebuffer{
	class Context{
		public:
			Context(Graphics::Context& graphics);
			~Context();

			Graphics::Context& graphics();
			FramebufferManager& manager();
			Loader& loader();
			spdlog::logger& logger();

		private:
			Graphics::Context& _graphics;
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<Loader> _loader;
			std::unique_ptr<FramebufferManager> _manager;
	};
}

#endif