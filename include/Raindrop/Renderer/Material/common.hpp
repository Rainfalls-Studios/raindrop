#ifndef __RAINDROP_RENDERER_MATERIAL_COMMON_HPP__
#define __RAINDROP_RENDERER_MATERIAL_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Renderer::Material{
	struct Context;
	struct Material;
	class SetLayout;
	class Buffer;
	class Pool;
	class Registry;
	class Materials;

	using MaterialID = std::size_t;
	static constexpr MaterialID PLACEHOLDER_MATERIAL = 0;
}

#endif