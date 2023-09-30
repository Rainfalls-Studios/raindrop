#ifndef __RAINDROP_GRAPHICS_UTILS_ENUMS_HPP__
#define __RAINDROP_GRAPHICS_UTILS_ENUMS_HPP__

#include <Raindrop/Graphics/Utils/common.hpp>

namespace Raindrop::Graphics::Utils{
	enum Flags{
		COLOR = 1 << 0,
		DEPTH = 1 << 1,
		STENCIL = 1 << 2,
		HDR = 1 << 3,
		COMPRESSED = 1 << 4,
		NORMAL = 1 << 5,
		STORAGE = 1 << 6,
		FLOAT = 1 << 7,
		SINT = 1 << 8,
		UINT = 1 << 9,
		PLANAR = 1 << 10,
		LINEAR = 1 << 11,
		GAMMA_CORRECTED = 1 << 12,
		R = 1 << 13,
		RG = 1 << 14,
		RGB = 1 << 15,
		RGBA = 1 << 16,
		BIT8 = 1 << 17,
		BIT16 = 1 << 18,
		BIT32 = 1 << 19,
		BIT48 = 1 << 20,
		BIT64 = 1 << 21,
		BIT96 = 1 << 22,
		BIT128 = 1 << 23,
		BIT192 = 1 << 24,
		BIT256 = 1 << 25,
		_FLAGS_COUNT = 26
	};
}

#endif