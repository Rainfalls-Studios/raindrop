#ifndef __RAINDROP_INTERNAL_COLOR_HPP__
#define __RAINDROP_INTERNAL_COLOR_HPP__

#include <Raindrop/Color.hpp>
#include <Raindrop_internal/Graphics/common.hpp>

namespace Raindrop{
	constexpr VkBlendFactor BlendFactorToVulkan(const Color::BlendFactor& factor){
		switch (factor){
			case Color::BlendFactor::ZERO: return VK_BLEND_FACTOR_ZERO;
			case Color::BlendFactor::ONE: return VK_BLEND_FACTOR_ONE;
			case Color::BlendFactor::SRC_COLOR: return VK_BLEND_FACTOR_SRC_COLOR;
			case Color::BlendFactor::ONE_MINUS_SRC_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case Color::BlendFactor::DST_COLOR: return VK_BLEND_FACTOR_DST_COLOR;
			case Color::BlendFactor::ONE_MINUS_DST_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case Color::BlendFactor::SRC_ALPHA: return VK_BLEND_FACTOR_SRC_ALPHA;
			case Color::BlendFactor::ONE_MINUS_SRC_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case Color::BlendFactor::DST_ALPHA: return VK_BLEND_FACTOR_DST_ALPHA;
			case Color::BlendFactor::ONE_MINUS_DST_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case Color::BlendFactor::CONSTANT_COLOR: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case Color::BlendFactor::ONE_MINUS_CONSTANT_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case Color::BlendFactor::CONSTANT_ALPHA: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case Color::BlendFactor::ONE_MINUS_CONSTANT_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case Color::BlendFactor::SRC_ALPHA_SATURATE: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case Color::BlendFactor::SRC1_COLOR: return VK_BLEND_FACTOR_SRC1_COLOR;
			case Color::BlendFactor::ONE_MINUS_SRC1_COLOR: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case Color::BlendFactor::SRC1_ALPHA: return VK_BLEND_FACTOR_SRC1_ALPHA;
			case Color::BlendFactor::ONE_MINUS_SRC1_ALPHA: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			default: return VK_BLEND_FACTOR_ZERO;
		}
	}

	constexpr Color::BlendFactor BlendFactorToRaindrop(const VkBlendFactor& factor){
		switch (factor){
			case VK_BLEND_FACTOR_ZERO: return Color::BlendFactor::ZERO;
			case VK_BLEND_FACTOR_ONE: return Color::BlendFactor::ONE;
			case VK_BLEND_FACTOR_SRC_COLOR: return Color::BlendFactor::SRC_COLOR;
			case VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR: return Color::BlendFactor::ONE_MINUS_SRC_COLOR;
			case VK_BLEND_FACTOR_DST_COLOR: return Color::BlendFactor::DST_COLOR;
			case VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR: return Color::BlendFactor::ONE_MINUS_DST_COLOR;
			case VK_BLEND_FACTOR_SRC_ALPHA: return Color::BlendFactor::SRC_ALPHA;
			case VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA: return Color::BlendFactor::ONE_MINUS_SRC_ALPHA;
			case VK_BLEND_FACTOR_DST_ALPHA: return Color::BlendFactor::DST_ALPHA;
			case VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA: return Color::BlendFactor::ONE_MINUS_DST_ALPHA;
			case VK_BLEND_FACTOR_CONSTANT_COLOR: return Color::BlendFactor::CONSTANT_COLOR;
			case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR: return Color::BlendFactor::ONE_MINUS_CONSTANT_COLOR;
			case VK_BLEND_FACTOR_CONSTANT_ALPHA: return Color::BlendFactor::CONSTANT_ALPHA;
			case VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA: return Color::BlendFactor::ONE_MINUS_CONSTANT_ALPHA;
			case VK_BLEND_FACTOR_SRC_ALPHA_SATURATE: return Color::BlendFactor::SRC_ALPHA_SATURATE;
			case VK_BLEND_FACTOR_SRC1_COLOR: return Color::BlendFactor::SRC1_COLOR;
			case VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR: return Color::BlendFactor::ONE_MINUS_SRC1_COLOR;
			case VK_BLEND_FACTOR_SRC1_ALPHA: return Color::BlendFactor::SRC1_ALPHA;
			case VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA: return Color::BlendFactor::ONE_MINUS_SRC1_ALPHA;
			default: return Color::BlendFactor::ZERO;
		}
	}

	constexpr VkColorComponentFlags ColorComponentsFlagsToVulkan(const Color::Components& components){
		VkColorComponentFlags out = 0;
		if (components.has(Color::Components::RED)) out |= VK_COLOR_COMPONENT_R_BIT;
		if (components.has(Color::Components::GREEN)) out |= VK_COLOR_COMPONENT_G_BIT;
		if (components.has(Color::Components::BLUE)) out |= VK_COLOR_COMPONENT_B_BIT;
		if (components.has(Color::Components::ALPHA)) out |= VK_COLOR_COMPONENT_A_BIT;
		return out;
	}

	constexpr Color::Components ColorComponentsFlagsToRaindrop(const VkColorComponentFlags& components){
		Color::Components out = Color::Components::NONE;
		if (components & VK_COLOR_COMPONENT_R_BIT) return Color::Components::RED;
		if (components & VK_COLOR_COMPONENT_G_BIT) return Color::Components::GREEN;
		if (components & VK_COLOR_COMPONENT_B_BIT) return Color::Components::BLUE;
		if (components & VK_COLOR_COMPONENT_A_BIT) return Color::Components::ALPHA;
		return out;
	}

	constexpr VkColorComponentFlagBits ColorComponentsFlagsBitsToVulkan(const Color::Components::Bits& bit){
		switch (bit){
			case Color::Components::RED: return VK_COLOR_COMPONENT_R_BIT;
			case Color::Components::GREEN: return VK_COLOR_COMPONENT_G_BIT;
			case Color::Components::BLUE: return VK_COLOR_COMPONENT_B_BIT;
			case Color::Components::ALPHA: return VK_COLOR_COMPONENT_A_BIT;
			default: return VK_COLOR_COMPONENT_R_BIT;
		}
	}


	constexpr Color::Components::Bits ColorComponentsFlagsBitsToVulkan(const VkColorComponentFlagBits& bit){
		switch (bit){
			case VK_COLOR_COMPONENT_R_BIT: return Color::Components::RED;
			case VK_COLOR_COMPONENT_G_BIT: return Color::Components::GREEN;
			case VK_COLOR_COMPONENT_B_BIT: return Color::Components::BLUE;
			case VK_COLOR_COMPONENT_A_BIT: return Color::Components::ALPHA;
			default: return Color::Components::NONE;
		}
	}

	constexpr VkBlendOp BlendOperationToVulkan(const Color::BlendOperation& op){
		switch (op){
			case Color::BlendOperation::ADD: return VK_BLEND_OP_ADD;
			case Color::BlendOperation::SUBTRACT: return VK_BLEND_OP_SUBTRACT;
			case Color::BlendOperation::REVERSE_SUBTRACT: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case Color::BlendOperation::MIN: return VK_BLEND_OP_MIN;
			case Color::BlendOperation::MAX: return VK_BLEND_OP_MAX;
			case Color::BlendOperation::ZERO: return VK_BLEND_OP_ZERO_EXT;
			case Color::BlendOperation::SRC: return VK_BLEND_OP_SRC_EXT;
			case Color::BlendOperation::DST: return VK_BLEND_OP_DST_EXT;
			case Color::BlendOperation::SRC_OVER: return VK_BLEND_OP_SRC_OVER_EXT;
			case Color::BlendOperation::DST_OVER: return VK_BLEND_OP_DST_OVER_EXT;
			case Color::BlendOperation::SRC_IN: return VK_BLEND_OP_SRC_IN_EXT;
			case Color::BlendOperation::DST_IN: return VK_BLEND_OP_DST_IN_EXT;
			case Color::BlendOperation::SRC_OUT: return VK_BLEND_OP_SRC_OUT_EXT;
			case Color::BlendOperation::DST_OUT: return VK_BLEND_OP_DST_OUT_EXT;
			case Color::BlendOperation::SRC_ATOP: return VK_BLEND_OP_SRC_ATOP_EXT;
			case Color::BlendOperation::DST_ATOP: return VK_BLEND_OP_DST_ATOP_EXT;
			case Color::BlendOperation::XOR: return VK_BLEND_OP_XOR_EXT;
			case Color::BlendOperation::MULTIPLY: return VK_BLEND_OP_MULTIPLY_EXT;
			case Color::BlendOperation::SCREEN: return VK_BLEND_OP_SCREEN_EXT;
			case Color::BlendOperation::OVERLAY: return VK_BLEND_OP_OVERLAY_EXT;
			case Color::BlendOperation::DARKEN: return VK_BLEND_OP_DARKEN_EXT;
			case Color::BlendOperation::LIGHTEN: return VK_BLEND_OP_LIGHTEN_EXT;
			case Color::BlendOperation::COLORDODGE: return VK_BLEND_OP_COLORDODGE_EXT;
			case Color::BlendOperation::COLORBURN: return VK_BLEND_OP_COLORBURN_EXT;
			case Color::BlendOperation::HARDLIGHT: return VK_BLEND_OP_HARDLIGHT_EXT;
			case Color::BlendOperation::SOFTLIGHT: return VK_BLEND_OP_SOFTLIGHT_EXT;
			case Color::BlendOperation::DIFFERENCE: return VK_BLEND_OP_DIFFERENCE_EXT;
			case Color::BlendOperation::EXCLUSION: return VK_BLEND_OP_EXCLUSION_EXT;
			case Color::BlendOperation::INVERT: return VK_BLEND_OP_INVERT_EXT;
			case Color::BlendOperation::INVERT_RGB: return VK_BLEND_OP_INVERT_RGB_EXT;
			case Color::BlendOperation::LINEARDODGE: return VK_BLEND_OP_LINEARDODGE_EXT;
			case Color::BlendOperation::LINEARBURN: return VK_BLEND_OP_LINEARBURN_EXT;
			case Color::BlendOperation::VIVIDLIGHT: return VK_BLEND_OP_VIVIDLIGHT_EXT;
			case Color::BlendOperation::LINEARLIGHT: return VK_BLEND_OP_LINEARLIGHT_EXT;
			case Color::BlendOperation::PINLIGHT: return VK_BLEND_OP_PINLIGHT_EXT;
			case Color::BlendOperation::HARDMIX: return VK_BLEND_OP_HARDMIX_EXT;
			case Color::BlendOperation::HSL_HUE: return VK_BLEND_OP_HSL_HUE_EXT;
			case Color::BlendOperation::HSL_SATURATION: return VK_BLEND_OP_HSL_SATURATION_EXT;
			case Color::BlendOperation::HSL_COLOR: return VK_BLEND_OP_HSL_COLOR_EXT;
			case Color::BlendOperation::HSL_LUMINOSITY: return VK_BLEND_OP_HSL_LUMINOSITY_EXT;
			case Color::BlendOperation::PLUS: return VK_BLEND_OP_PLUS_EXT;
			case Color::BlendOperation::PLUS_CLAMPED: return VK_BLEND_OP_PLUS_CLAMPED_EXT;
			case Color::BlendOperation::PLUS_CLAMPED_ALPHA: return VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT;
			case Color::BlendOperation::PLUS_DARKER: return VK_BLEND_OP_PLUS_DARKER_EXT;
			case Color::BlendOperation::MINUS: return VK_BLEND_OP_MINUS_EXT;
			case Color::BlendOperation::MINUS_CLAMPED: return VK_BLEND_OP_MINUS_CLAMPED_EXT;
			case Color::BlendOperation::CONTRAST: return VK_BLEND_OP_CONTRAST_EXT;
			case Color::BlendOperation::INVERT_OVG: return VK_BLEND_OP_INVERT_OVG_EXT;
			case Color::BlendOperation::RED: return VK_BLEND_OP_RED_EXT;
			case Color::BlendOperation::GREEN: return VK_BLEND_OP_GREEN_EXT;
			case Color::BlendOperation::BLUE: return VK_BLEND_OP_BLUE_EXT;
			default: return VK_BLEND_OP_ADD;
		}
	}

	constexpr Color::BlendOperation BlendOperationToRaindrop(const VkBlendOp& op){
		switch (op){
			case VK_BLEND_OP_ADD: return Color::BlendOperation::ADD;
			case VK_BLEND_OP_SUBTRACT: return Color::BlendOperation::SUBTRACT;
			case VK_BLEND_OP_REVERSE_SUBTRACT: return Color::BlendOperation::REVERSE_SUBTRACT;
			case VK_BLEND_OP_MIN: return Color::BlendOperation::MIN;
			case VK_BLEND_OP_MAX: return Color::BlendOperation::MAX;
			case VK_BLEND_OP_ZERO_EXT: return Color::BlendOperation::ZERO;
			case VK_BLEND_OP_SRC_EXT: return Color::BlendOperation::SRC;
			case VK_BLEND_OP_DST_EXT: return Color::BlendOperation::DST;
			case VK_BLEND_OP_SRC_OVER_EXT: return Color::BlendOperation::SRC_OVER;
			case VK_BLEND_OP_DST_OVER_EXT: return Color::BlendOperation::DST_OVER;
			case VK_BLEND_OP_SRC_IN_EXT: return Color::BlendOperation::SRC_IN;
			case VK_BLEND_OP_DST_IN_EXT: return Color::BlendOperation::DST_IN;
			case VK_BLEND_OP_SRC_OUT_EXT: return Color::BlendOperation::SRC_OUT;
			case VK_BLEND_OP_DST_OUT_EXT: return Color::BlendOperation::DST_OUT;
			case VK_BLEND_OP_SRC_ATOP_EXT: return Color::BlendOperation::SRC_ATOP;
			case VK_BLEND_OP_DST_ATOP_EXT: return Color::BlendOperation::DST_ATOP;
			case VK_BLEND_OP_XOR_EXT: return Color::BlendOperation::XOR;
			case VK_BLEND_OP_MULTIPLY_EXT: return Color::BlendOperation::MULTIPLY;
			case VK_BLEND_OP_SCREEN_EXT: return Color::BlendOperation::SCREEN;
			case VK_BLEND_OP_OVERLAY_EXT: return Color::BlendOperation::OVERLAY;
			case VK_BLEND_OP_DARKEN_EXT: return Color::BlendOperation::DARKEN;
			case VK_BLEND_OP_LIGHTEN_EXT: return Color::BlendOperation::LIGHTEN;
			case VK_BLEND_OP_COLORDODGE_EXT: return Color::BlendOperation::COLORDODGE;
			case VK_BLEND_OP_COLORBURN_EXT: return Color::BlendOperation::COLORBURN;
			case VK_BLEND_OP_HARDLIGHT_EXT: return Color::BlendOperation::HARDLIGHT;
			case VK_BLEND_OP_SOFTLIGHT_EXT: return Color::BlendOperation::SOFTLIGHT;
			case VK_BLEND_OP_DIFFERENCE_EXT: return Color::BlendOperation::DIFFERENCE;
			case VK_BLEND_OP_EXCLUSION_EXT: return Color::BlendOperation::EXCLUSION;
			case VK_BLEND_OP_INVERT_EXT: return Color::BlendOperation::INVERT;
			case VK_BLEND_OP_INVERT_RGB_EXT: return Color::BlendOperation::INVERT_RGB;
			case VK_BLEND_OP_LINEARDODGE_EXT: return Color::BlendOperation::LINEARDODGE;
			case VK_BLEND_OP_LINEARBURN_EXT: return Color::BlendOperation::LINEARBURN;
			case VK_BLEND_OP_VIVIDLIGHT_EXT: return Color::BlendOperation::VIVIDLIGHT;
			case VK_BLEND_OP_LINEARLIGHT_EXT: return Color::BlendOperation::LINEARLIGHT;
			case VK_BLEND_OP_PINLIGHT_EXT: return Color::BlendOperation::PINLIGHT;
			case VK_BLEND_OP_HARDMIX_EXT: return Color::BlendOperation::HARDMIX;
			case VK_BLEND_OP_HSL_HUE_EXT: return Color::BlendOperation::HSL_HUE;
			case VK_BLEND_OP_HSL_SATURATION_EXT: return Color::BlendOperation::HSL_SATURATION;
			case VK_BLEND_OP_HSL_COLOR_EXT: return Color::BlendOperation::HSL_COLOR;
			case VK_BLEND_OP_HSL_LUMINOSITY_EXT: return Color::BlendOperation::HSL_LUMINOSITY;
			case VK_BLEND_OP_PLUS_EXT: return Color::BlendOperation::PLUS;
			case VK_BLEND_OP_PLUS_CLAMPED_EXT: return Color::BlendOperation::PLUS_CLAMPED;
			case VK_BLEND_OP_PLUS_CLAMPED_ALPHA_EXT: return Color::BlendOperation::PLUS_CLAMPED_ALPHA;
			case VK_BLEND_OP_PLUS_DARKER_EXT: return Color::BlendOperation::PLUS_DARKER;
			case VK_BLEND_OP_MINUS_EXT: return Color::BlendOperation::MINUS;
			case VK_BLEND_OP_MINUS_CLAMPED_EXT: return Color::BlendOperation::MINUS_CLAMPED;
			case VK_BLEND_OP_CONTRAST_EXT: return Color::BlendOperation::CONTRAST;
			case VK_BLEND_OP_INVERT_OVG_EXT: return Color::BlendOperation::INVERT_OVG;
			case VK_BLEND_OP_RED_EXT: return Color::BlendOperation::RED;
			case VK_BLEND_OP_GREEN_EXT: return Color::BlendOperation::GREEN;
			case VK_BLEND_OP_BLUE_EXT: return Color::BlendOperation::BLUE;
			default: return Color::BlendOperation::ADD;
		}
	}

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