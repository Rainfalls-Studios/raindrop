#ifndef __RAINDROP_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CONTEXT_HPP__

#include "Core/Context.hpp"
#include "Window/Context.hpp"
#include "Sync/Context.hpp"

namespace Raindrop::Graphics{
	struct Context{
		Core::Context core;
		Window::Context window;
		Sync::Context sync;

		void initialize();
	};
}

#endif