#ifndef __RAINDROP_GRAPHICS_MODELS_VERTEX_HPP__
#define __RAINDROP_GRAPHICS_MODELS_VERTEX_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::Models{
	struct Vertex{
		Maths::vec3 position;
		Maths::vec3 normal;
		Maths::vec3 color;
		Maths::vec2 UV;
	};
}

#endif