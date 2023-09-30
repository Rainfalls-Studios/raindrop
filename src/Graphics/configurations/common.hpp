#ifndef __RAINDROP_GRAPHICS_CONFIGURATION_COMMON_HPP__
#define __RAINDROP_GRAPHICS_CONFIGURATION_COMMON_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/FormatUtilities.hpp>

namespace Raindrop::Graphics{
	struct AttachmentInfo{
		VkAttachmentDescription description;
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;

		VkFormatFeatureFlags requiredFeatures;
		uint32_t flags;
		
		VkClearValue clear;
	};
}

#endif