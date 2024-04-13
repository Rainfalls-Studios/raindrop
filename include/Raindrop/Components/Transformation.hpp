#ifndef __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__
#define __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__

#include "common.hpp"

namespace Raindrop::Components{
	struct Transformation{
		Transformation();

		Maths::mat4 matrix;

		Maths::vec3 translation;
		Maths::vec3 scale;
		Maths::quat rotation;

		void updateMatrix();
	};
}

#endif