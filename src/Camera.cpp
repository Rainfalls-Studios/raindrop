#include <Raindrop/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop{
	Camera::Camera() : 
		_projection(1.f),
		_viewProjection(1.f),
		_aspectRatio{1.f},
		_fov{80.f}
	{
		updateProjection();
		updateTransform();
	}

	Camera::~Camera(){}

	void Camera::updateTransform(){
		_transform.updateMatrix();
		updateViewProjection();
	}

	void Camera::updateProjection(){
		_projection = Maths::perspective(_fov, _aspectRatio, 0.1f, 15000.f);
		updateViewProjection();
	}

	void Camera::updateViewProjection(){
		_viewProjection = _projection * Maths::inverse(_transform.matrix);
	}

	void Camera::translate(const Maths::vec3& translation){
		_transform.translation += translation;
		updateTransform();
	}

	void Camera::scale(const Maths::vec3& factor){
		_transform.scale *= factor;
		updateTransform();
	}

	void Camera::rotate(const Maths::quat& rotation){
		_transform.rotation *= rotation;
		_transform.rotation = Maths::normalize(_transform.rotation);
		updateTransform();
	}

	void Camera::setTranslation(const Maths::vec3& translation){
		_transform.translation = translation;
		updateTransform();
	}

	void Camera::setScale(const Maths::vec4& scale){
		_transform.scale = scale;
		updateTransform();
	}

	void Camera::setRotation(const Maths::quat& rotation){
		_transform.rotation = rotation;
		updateTransform();
	}

	void Camera::setAspectRatio(float ratio){
		_aspectRatio = ratio;
		updateProjection();
	}

	void Camera::setFov(float fov){
		_fov = fov;
		updateProjection();
	}

	const Maths::mat4& Camera::projection() const{
		return _projection;
	}

	const Maths::mat4& Camera::transform() const{
		return _transform.matrix;
	}

	const Maths::mat4& Camera::viewProjection() const{
		return _viewProjection;
	}

	const Maths::vec3& Camera::translation() const{
		return _transform.translation;
	}

	const Maths::vec3& Camera::scale() const{
		return _transform.scale;
	}

	const Maths::quat& Camera::rotation() const{
		return _transform.rotation;
	}
}