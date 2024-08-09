#ifndef __RAINDROP_GRAPHICS_WINDOW_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_CONTEXT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Window.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"

namespace Raindrop::Graphics::Window{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		Window window;
		Surface surface;
		Swapchain swapchain;
		
		void createLogger();
	};
}

#endif