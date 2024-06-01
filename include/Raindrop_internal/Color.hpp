#ifndef __RAINDROP_INTERNAL_COLOR_HPP__
#define __RAINDROP_INTERNAL_COLOR_HPP__

#include <Raindrop/Color.hpp>
#include <Raindrop_internal/Graphics/common.hpp>

namespace Raindrop{
	VkBlendFactor toVulkan(const Color::BlendFactor& factor);
	VkColorComponentFlags toVulkan(const Color::Components& components);
	VkColorComponentFlagBits toVulkan(const Color::Components::Bits& component);
	VkBlendOp toVulkan(const Color::BlendOperation& op);
	VkComponentSwizzle toVulkan(const Color::Swizzle& s);
}

#endif