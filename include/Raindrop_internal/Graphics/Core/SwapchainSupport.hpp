#ifndef __RAINDROP_INTERNAL_GRAPHICS_CORE_SWAPCHAIN_SUPPORT_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CORE_SWAPCHAIN_SUPPORT_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics::Core{
	struct SwapchainSupport{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		bool supported;
	};
}

#endif