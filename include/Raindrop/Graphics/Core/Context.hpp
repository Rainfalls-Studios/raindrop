#ifndef __RAINDROP_GRAPHICS_CORE_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CORE_CONTEXT_HPP__

#include "common.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Core{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		VkAllocationCallbacks* allocationCallbacks;
		Instance instance;
		PhysicalDevice physicalDevice;
		Device device;

		void initialize();
		void createLogger();
	};
}

#endif