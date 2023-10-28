#ifndef __RAINDROP_GRAPHICS_CAMERAS_COMMON_HPP__
#define __RAINDROP_GRAPHICS_CAMERAS_COMMON_HPP__

#include <glm/gtc/quaternion.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Projections{
	class PerspectiveProjection;
	class OrthographicProjection;
	class Projection;

	enum ProjectionType{
		PERSPECTIVE,
		ORTHOGRAPHIC,	
	};
}

#endif