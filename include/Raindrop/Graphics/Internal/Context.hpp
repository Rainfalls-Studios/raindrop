#ifndef __RAINDROP_GRAPHICS_INTERNAL_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Internal/common.hpp>
#include <Raindrop/Graphics/Internal/Instance.hpp>
#include <Raindrop/Graphics/Internal/Window.hpp>
#include <Raindrop/Graphics/Internal/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Internal/Device.hpp>
#include <Raindrop/Graphics/Internal/Swapchain.hpp>

namespace Raindrop::Graphics::Internal{
	struct Context{
		Graphics::Context& graphics;
		spdlog::logger logger;

		Window window;
		Instance instance;
		PhysicalDevice physicalDevice;
		Device device;
		Swapchain swapchain;

		Context(Graphics::Context& graphics);
	};
}

#endif