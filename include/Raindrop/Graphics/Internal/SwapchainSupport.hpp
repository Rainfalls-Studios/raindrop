#ifndef __RAINDROP_GRAPHICS_INTERNAL_SWAPCHAIN_SUPPORT_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_SWAPCHAIN_SUPPORT_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	struct SwapchainSupport{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		bool supported;
	};
}

#endif