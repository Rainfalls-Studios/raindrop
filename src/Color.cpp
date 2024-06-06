#include <Raindrop_internal/Color.hpp>

namespace Raindrop{
	VkBlendFactor toVulkan(const Color::BlendFactor& factor){
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
		}
		throw std::runtime_error("invalid color blend factor bit");
	}

	VkColorComponentFlags toVulkan(const Color::Components& components){
		VkColorComponentFlags out = 0;
		if (components.has(Color::Components::RED)) out |= VK_COLOR_COMPONENT_R_BIT;
		if (components.has(Color::Components::GREEN)) out |= VK_COLOR_COMPONENT_G_BIT;
		if (components.has(Color::Components::BLUE)) out |= VK_COLOR_COMPONENT_B_BIT;
		if (components.has(Color::Components::ALPHA)) out |= VK_COLOR_COMPONENT_A_BIT;
		return out;
	}

	VkBlendOp toVulkan(const Color::BlendOperation& op){
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
		}
		throw std::runtime_error("Invalid color blend operation");
	}

	VkComponentSwizzle toVulkan(const Color::Swizzle& s){
		throw std::runtime_error("Invalid color component swizzle");
	}
}