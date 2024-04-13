#ifndef __RAINDROP_MATHS_VECTOR_HPP__
#define __RAINDROP_MATHS_VECTOR_HPP__

#include "common.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Raindrop::Maths{
	using namespace glm;

	// Rotates a vector by a quaternion
	template<typename T>
	inline vec<3, T> rotateVector(const vec<3, T>& v, const quat& rotation) {
		quat conj = conjugate(rotation);
		quat rotated = rotation * quat(0, v) * conj;
		return vec<3, T>(rotated.x, rotated.y, rotated.z);
	}
}

#endif