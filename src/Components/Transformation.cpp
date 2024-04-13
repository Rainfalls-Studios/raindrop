#include <Raindrop/Components/Transformation.hpp>

namespace Raindrop::Components{
	Transformation::Transformation() : 
		matrix(1.f),
		translation(0.f),
		scale(1.f),
		rotation(){}
	
	void Transformation::updateMatrix(){
		Maths::mat4 translationMatrix = Maths::translate(Maths::mat4(1.f), translation);
		Maths::mat4 scaleMatrix = Maths::scale(Maths::mat4(1.f), scale);
		Maths::mat4 rotationMatrix = Maths::toMat4(rotation);

		matrix = translationMatrix * rotationMatrix * scaleMatrix;
	}
}