#ifndef __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__
#define __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__

#include "common.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Raindrop::Components{
	struct Transformation{
		Transformation();

		glm::mat4 matrix;

		glm::vec3 translation;
		glm::vec3 scale;
		glm::quat rotation;

		void updateMatrix();
	};
}

#endif