#ifndef __RAINDROP_COLOR_HPP__
#define __RAINDROP_COLOR_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"

namespace Raindrop{
	class Color{
		public:
			enum class ComponentsBits{
				NONE = 0,
				RED = 1 << 0,
				GREEN = 1 << 1,
				BLUE = 1 << 2,
				ALPHA = 1 << 3,

				R = RED,
				G = GREEN,
				B = BLUE,
				A = ALPHA,

				RG = RED | GREEN,
				RGB = RED | GREEN | BLUE,
				RGBA = RED | GREEN | BLUE | ALPHA
			};
			RAINDROP_FLAGS_CLASS(Components, ComponentsBits);

			enum class Swizzle{
				RED,
				GREEN,
				BLUE,
				ALPHA,
				ONE,
				ZERO,
				IDENTITY
			};

			enum class BlendFactor{
				ZERO,
				ONE,
				SRC_COLOR,
				ONE_MINUS_SRC_COLOR,
				DST_COLOR,
				ONE_MINUS_DST_COLOR,
				SRC_ALPHA,
				ONE_MINUS_SRC_ALPHA,
				DST_ALPHA,
				ONE_MINUS_DST_ALPHA,
				CONSTANT_COLOR,
				ONE_MINUS_CONSTANT_COLOR,
				CONSTANT_ALPHA,
				ONE_MINUS_CONSTANT_ALPHA,
				SRC_ALPHA_SATURATE,
				SRC1_COLOR,
				ONE_MINUS_SRC1_COLOR,
				SRC1_ALPHA,
				ONE_MINUS_SRC1_ALPHA,
			};

			enum class BlendOperation{
				ADD,
				SUBTRACT,
				REVERSE_SUBTRACT,
				MIN,
				MAX,
				ZERO,
				SRC,
				DST,
				SRC_OVER,
				DST_OVER,
				SRC_IN,
				DST_IN,
				SRC_OUT,
				DST_OUT,
				SRC_ATOP,
				DST_ATOP,
				XOR,
				MULTIPLY,
				SCREEN,
				OVERLAY,
				DARKEN,
				LIGHTEN,
				COLORDODGE,
				COLORBURN,
				HARDLIGHT,
				SOFTLIGHT,
				DIFFERENCE,
				EXCLUSION,
				INVERT,
				INVERT_RGB,
				LINEARDODGE,
				LINEARBURN,
				VIVIDLIGHT,
				LINEARLIGHT,
				PINLIGHT,
				HARDMIX,
				HSL_HUE,
				HSL_SATURATION,
				HSL_COLOR,
				HSL_LUMINOSITY,
				PLUS,
				PLUS_CLAMPED,
				PLUS_CLAMPED_ALPHA,
				PLUS_DARKER,
				MINUS,
				MINUS_CLAMPED,
				CONTRAST,
				INVERT_OVG,
				RED,
				GREEN,
				BLUE,
			};

		private:
			
	};
}

#endif