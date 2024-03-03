#ifndef __RAINDROP_RENDERER_CONTEXT_HPP__
#define __RAINDROP_RENDERER_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Instance.hpp"
#include "Core/Device.hpp"
#include "Core/PhysicalDevice.hpp"
#include "Core/Window.hpp"

namespace Raindrop::Renderer{
	struct Context{
		Context(::Raindrop::Context& core);
		~Context() = default;

		::Raindrop::Context& core;
		VkAllocationCallbacks* allocationCallbacks;

		Core::Window window;
		Core::Instance instance;
		Core::PhysicalDevice physicalDevice;
		Core::Device device;

	};
}

#endif