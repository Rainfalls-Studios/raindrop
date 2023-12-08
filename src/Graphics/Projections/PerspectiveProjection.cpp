// #include <Raindrop/Graphics/Projections/PerspectiveProjection.hpp>

// #include <glm/gtc/matrix_transform.hpp>

// namespace Raindrop::Graphics::Projections{
// 	PerspectiveProjection::PerspectiveProjection(){
// 		setProjection(75.f, 0.1, 150.f, 1.f);
// 	}

// 	void PerspectiveProjection::setProjection(float fov, float nearClip, float farClip, float aspectRatio){
// 		_aspectRatio = aspectRatio;
// 		_fov = fov;
// 		_farClip = farClip;
// 		_nearClip = nearClip;

// 		_projectionMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
// 	}
	
// 	void PerspectiveProjection::update(){
// 		glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
// 		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);

// 		glm::mat4 transform = translationMatrix * rotationMatrix;
// 		_viewMatrix = glm::inverse(transform);
// 		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
// 	}
// }