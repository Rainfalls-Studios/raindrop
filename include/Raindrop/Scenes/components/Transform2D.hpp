#ifndef __RAINDROP_SCENES_COMPONENTS_TRANSFORM_2D_HPP__
#define __RAINDROP_SCENES_COMPONENTS_TRANSFORM_2D_HPP__

#include "../pch.pch"
#include <glm/gtx/matrix_transform_2d.hpp>

namespace Raindrop::Scenes::Components{
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

#endif