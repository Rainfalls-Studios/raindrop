#include <Raindrop/Components/Transformation.hpp>

namespace Raindrop::Components{
	Transformation::Transformation() : 
		matrix(1.f),
		translation(0.f),
		scale(1.f),
		rotation(1.f, 0.f, 0.f, 0.f){}
	
	void Transformation::updateMatrix(){
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), translation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);
		glm::mat4 rotationMatrix = glm::toMat4(rotation);

		matrix = translationMatrix * rotationMatrix * scaleMatrix;
	}
}