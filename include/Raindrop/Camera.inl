// #ifndef __RAINDROP_CAMERA_INL_HPP__
// #define __RAINDROP_CAMERA_INL_HPP__

// #include "Camera.hpp"

// namespace Raindrop{
// 	inline void Camera::translate(const Maths::vec3& translation){
// 		_transform.getTranslation() += translation;
// 		updateTransform();
// 	}

// 	inline void Camera::scale(const Maths::vec3& factor){
// 		_transform.getScale() *= factor;
// 		updateTransform();
// 	}

// 	inline void Camera::rotate(const Maths::quat& rotation){
// 		_transform.getRotation() *= rotation;
// 		_transform.setRotation(Maths::normalize(_transform.getRotation()));

// 		updateTransform();
// 	}

// 	inline void Camera::setTranslation(const Maths::vec3& translation){
// 		_transform.setTranslation(translation);
// 		updateTransform();
// 	}

// 	inline void Camera::setScale(const Maths::vec4& scale){
// 		_transform.setScale(scale);
// 		updateTransform();
// 	}

// 	inline void Camera::setRotation(const Maths::quat& rotation){
// 		_transform.setRotation(rotation);
// 		updateTransform();
// 	}

// 	inline void Camera::setAspectRatio(float ratio){
// 		_aspectRatio = ratio;
// 		updateProjection();
// 	}

// 	inline void Camera::setFov(float fov){
// 		_fov = fov;
// 		updateProjection();
// 	}
	
// 	inline const Maths::mat4& Camera::getProjection() const{
// 		return _projection;
// 	}

// 	inline const Maths::mat4& Camera::getTransform() const{
// 		return _transform.getMatrix();
// 	}

// 	inline const Maths::mat4& Camera::getViewProjection() const{
// 		return _viewProjection;
// 	}

// 	inline const Maths::vec3& Camera::getTranslation() const{
// 		return _transform.getTranslation();
// 	}

// 	inline const Maths::vec3& Camera::getScale() const{
// 		return _transform.getScale();
// 	}

// 	inline const Maths::quat& Camera::getRotation() const{
// 		return _transform.getRotation();
// 	}
// }

// #endif