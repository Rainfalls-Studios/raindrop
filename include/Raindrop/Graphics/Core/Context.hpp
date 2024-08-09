#ifndef __RAINDROP_GRAPHICS_CORE_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CORE_CONTEXT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "Device.hpp"

namespace Raindrop::Graphics::Core{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		VkAllocationCallbacks* allocationCallbacks;
		Instance instance;
		PhysicalDevice physicalDevice;
		Device device;

		void createLogger();
	};
}

#endif