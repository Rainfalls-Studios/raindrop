#ifndef __RAINDROP_COMPONENTS_TRANSFORMATION_INL__
#define __RAINDROP_COMPONENTS_TRANSFORMATION_INL__

#include "Transformation.hpp"

namespace Raindrop::Components{
	inline void Transformation::setMatrix(const Maths::mat4& matrix){
		_matrix = matrix;
	}

	inline Maths::mat4& Transformation::getMatrix(){
		return _matrix;
	}

	inline const Maths::mat4& Transformation::getMatrix() const{
		return _matrix;
	}

	inline void Transformation::setTranslation(const Maths::vec3& translation){
		_translation = translation;
	}

	inline Maths::vec3& Transformation::getTranslation(){
		return _translation;
	}

	inline const Maths::vec3& Transformation::getTranslation() const{
		return _translation;
	}

	inline void Transformation::setScale(const Maths::vec3& scale){
		_scale = scale;
	}

	inline Maths::vec3& Transformation::getScale(){
		return _scale;
	}

	inline const Maths::vec3& Transformation::getScale() const{
		return _scale;
	}
	
	inline void Transformation::setRotation(const Maths::quat& rotation){
		_rotation = rotation;
	}

	inline Maths::quat& Transformation::getRotation(){
		return _rotation;
	}

	inline const Maths::quat& Transformation::getRotation() const{
		return _rotation;
	}
}

#endif