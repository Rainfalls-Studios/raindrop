#pragma once

#include "Raindrop/pch.pch"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Raindrop::Core::Components{
	struct Transform3D{
		glm::vec3 translation;
		glm::vec3 scale;
		glm::quat rotation;

		glm::mat4 matrix;

		void updateMatrix(){
			const glm::mat4 translationMat = glm::translate(glm::mat4(1.f), translation);
			const glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), scale);
			const glm::mat4 rotationMat = glm::mat4_cast(rotation);

			matrix = translationMat * rotationMat * scaleMat;
		}
	};
}