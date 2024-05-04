#include <Raindrop/Components/Transformation.hpp>

namespace Raindrop::Components{
	Transformation::Transformation() : 
		_matrix(1.f),
		_translation(0.f),
		_scale(1.f),
		_rotation(){}
	
	void Transformation::updateMatrix(){
		Maths::mat4 translationMatrix = Maths::translate(Maths::mat4(1.f), _translation);
		Maths::mat4 scaleMatrix = Maths::scale(Maths::mat4(1.f), _scale);
		Maths::mat4 rotationMatrix = Maths::toMat4(_rotation);

		_matrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

	
}