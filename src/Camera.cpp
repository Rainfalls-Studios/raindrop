#include <Raindrop/Camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop{
	Camera::Camera() : 
		_projection(1.f),
		_transform(1.f),
		_viewTranform(1.f),
		_translation(0.f),
		_scale(1.f),
		_rotation(0.f, {1.f, 0.f, 0.f}),
		_aspectRatio{1.f},
		_fov{80.f}
	{}

	Camera::~Camera(){}

	void Camera::updateTransform(){
		glm::mat4 translation = glm::translate(glm::mat4(1.f), _translation);
		glm::mat4 scale = glm::scale(glm::mat4(1.f), _scale);
		glm::mat4 rotation = glm::toMat4(_rotation);

		_transform = translation * rotation * scale;
		updateViewTransform();
	}

	void Camera::updateProjection(){
		_projection = glm::perspective(_fov, _aspectRatio, 0.1f, 500.f);
		updateViewTransform();
	}

	void Camera::updateViewTransform(){
		_viewTranform = _projection * glm::inverse(_transform);
	}

	void Camera::translate(const glm::vec3& translation){
		_translation += translation;
		updateTransform();
	}

	void Camera::scale(const glm::vec3& factor){
		_scale *= factor;
		updateTransform();
	}

	void Camera::rotate(const glm::quat& rotation){
		_rotation += rotation;
		updateTransform();
	}

	void Camera::setTranslation(const glm::vec3& translation){
		_translation = translation;
		updateTransform();
	}

	void Camera::setScale(const glm::vec4& scale){
		_scale = scale;
		updateTransform();
	}

	void Camera::setRotation(const glm::quat& rotation){
		_rotation = rotation;
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
		return _transform;
	}

	const glm::mat4& Camera::viewTransform() const{
		return _viewTranform;
	}

	const glm::vec3& Camera::translation() const{
		return _translation;
	}

	const glm::vec3& Camera::scale() const{
		return _scale;
	}

	const glm::quat& Camera::rotation() const{
		return _rotation;
	}
}