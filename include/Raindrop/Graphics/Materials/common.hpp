#ifndef __RAINDROP_GRAPHICS_MATERIAL_COMMON_HPP__
#define __RAINDROP_GRAPHICS_MATERIAL_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Graphics::Materials{
	struct Context;
	struct Material;
	class SetLayout;
	class Buffer;
	class Pool;
	class Registry;
	class Materials;

	using MaterialID = std::size_t;
	static constexpr MaterialID PLACEHOLDER_MATERIAL_ID = 0;
}

#endif