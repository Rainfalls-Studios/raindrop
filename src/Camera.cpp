#include <Raindrop/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop{
	Camera::Camera() : 
		_projection(1.f),
		_viewTranform(1.f),
		_aspectRatio{1.f},
		_fov{80.f}
	{
		updateProjection();
		updateTransform();
	}

	Camera::~Camera(){}

	void Camera::updateTransform(){
		_transform.updateMatrix();
		updateViewTransform();
	}

	void Camera::updateProjection(){
		_projection = glm::perspective(_fov, _aspectRatio, 0.1f, 15000.f);
		updateViewTransform();
	}

	void Camera::updateViewTransform(){
		_viewTranform = _projection * glm::inverse(_transform.matrix);
	}

	void Camera::translate(const glm::vec3& translation){
		_transform.translation += translation;
		updateTransform();
	}

	void Camera::scale(const glm::vec3& factor){
		_transform.scale *= factor;
		updateTransform();
	}

	void Camera::rotate(const glm::quat& rotation){
		_transform.rotation *= rotation;
		_transform.rotation = glm::normalize(_transform.rotation);
		updateTransform();
	}

	void Camera::setTranslation(const glm::vec3& translation){
		_transform.translation = translation;
		updateTransform();
	}

	void Camera::setScale(const glm::vec4& scale){
		_transform.scale = scale;
		updateTransform();
	}

	void Camera::setRotation(const glm::quat& rotation){
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

	const glm::mat4& Camera::projection() const{
		return _projection;
	}

	const glm::mat4& Camera::transform() const{
		return _transform.matrix;
	}

	const glm::mat4& Camera::viewTransform() const{
		return _viewTranform;
	}

	const glm::vec3& Camera::translation() const{
		return _transform.translation;
	}

	const glm::vec3& Camera::scale() const{
		return _transform.scale;
	}

	const glm::quat& Camera::rotation() const{
		return _transform.rotation;
	}
}