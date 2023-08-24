#ifndef __RAINDROP_GRAPHICS_CONFIGURATION_COMMON_HPP__
#define __RAINDROP_GRAPHICS_CONFIGURATION_COMMON_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	struct AttachmentInfo{
		VkAttachmentDescription description;
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;

		// if none of the formats are available, throw an exception
		std::vector<VkFormat> formats; // ordered from best to worst case
		VkFormatFeatureFlags requiredFeatures;
		VkClearValue clear;
	};
}

#endif