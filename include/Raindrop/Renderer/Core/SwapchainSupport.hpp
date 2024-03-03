#ifndef __RAINDROP_RENDERER_CORE_SWAPCHAIN_SUPPORT_HPP__
#define __RAINDROP_RENDERER_CORE_SWAPCHAIN_SUPPORT_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Core{
	struct SwapchainSupport{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
		bool supported;
	};
}

#endif