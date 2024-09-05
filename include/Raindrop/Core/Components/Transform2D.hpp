#pragma once

#include "Raindrop/pch.pch"
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Raindrop::Core::Components{
	struct Transform2D{
		glm::vec2 translation;
		glm::vec2 scale;
		float rotation;

		glm::mat3 matrix;
		void updateMatrix(){
			const glm::mat3 translationMat = glm::translate(glm::mat3(1.f), translation);
			const glm::mat3 scaleMat = glm::scale(glm::mat3(1.f), scale);
			const glm::mat3 rotationMat = glm::rotate(glm::mat3(1.f), rotation);

			matrix = translationMat * rotationMat * scaleMat;
		}
	};
}