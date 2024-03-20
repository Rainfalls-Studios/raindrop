#ifndef __RAINDROP_RENDERER_MODEL_VERTEX_HPP__
#define __RAINDROP_RENDERER_MODEL_VERTEX_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Model{
	struct Vertex{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 UV;
	};
}

#endif