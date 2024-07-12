#ifndef __RAINDROP_GRAPHICS_WINDOW_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_CONTEXT_HPP__

#include "Window.hpp"
#include "Surface.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Window{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		Window window;
		Surface surface;


		void createLogger();
	};
}

#endif