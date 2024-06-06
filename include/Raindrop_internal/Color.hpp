#ifndef __RAINDROP_INTERNAL_COLOR_HPP__
#define __RAINDROP_INTERNAL_COLOR_HPP__

#include <Raindrop/Color.hpp>
#include <Raindrop_internal/Graphics/common.hpp>

namespace Raindrop{
	VkBlendFactor toVulkan(const Color::BlendFactor& factor);
	VkColorComponentFlags toVulkan(const Color::Components& components);
	VkColorComponentFlagBits toVulkan(const Color::Components::Bits& component);
	VkBlendOp toVulkan(const Color::BlendOperation& op);

	constexpr VkComponentSwizzle ComponentSwizzleToVulkan(const Color::Swizzle& s){
		switch (s){
			case Color::Swizzle::RED: return VK_COMPONENT_SWIZZLE_R;
			case Color::Swizzle::GREEN: return VK_COMPONENT_SWIZZLE_G;
			case Color::Swizzle::BLUE: return VK_COMPONENT_SWIZZLE_B;
			case Color::Swizzle::ALPHA: return VK_COMPONENT_SWIZZLE_A;
			case Color::Swizzle::ONE: return VK_COMPONENT_SWIZZLE_ONE;
			case Color::Swizzle::ZERO: return VK_COMPONENT_SWIZZLE_ZERO;
			case Color::Swizzle::IDENTITY: return VK_COMPONENT_SWIZZLE_IDENTITY;
			default: return VK_COMPONENT_SWIZZLE_IDENTITY;
		}
	}

	constexpr Color::Swizzle ComponentSwizzleToRaindrop(const VkComponentSwizzle& s){
		switch (s){
			case VK_COMPONENT_SWIZZLE_R: return Color::Swizzle::RED;
			case VK_COMPONENT_SWIZZLE_G: return Color::Swizzle::GREEN;
			case VK_COMPONENT_SWIZZLE_B: return Color::Swizzle::BLUE;
			case VK_COMPONENT_SWIZZLE_A: return Color::Swizzle::ALPHA;
			case VK_COMPONENT_SWIZZLE_ONE: return Color::Swizzle::ONE;
			case VK_COMPONENT_SWIZZLE_ZERO: return Color::Swizzle::ZERO;
			case VK_COMPONENT_SWIZZLE_IDENTITY: return Color::Swizzle::IDENTITY;
			default: return Color::Swizzle::IDENTITY;
		}
	}
}

#endif