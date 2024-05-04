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
		_viewProjection = _projection * Maths::inverse(_transform.getMatrix());
	}
}