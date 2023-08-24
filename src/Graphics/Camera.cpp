#include <Raindrop/Graphics/Camera.hpp>
#include <Raindrop/Graphics/Projections/OrthographicProjection.hpp>
#include <Raindrop/Graphics/Projections/PerspectiveProjection.hpp>

namespace Raindrop::Graphics{
	Camera::Camera(GraphicsContext& context, uint32_t width, uint32_t height) :
		_context{context},
		_framebuffer{_context, width, height}{

		perspective().setProjection(
			75.f,
			0.1,
			150.f,
			static_cast<float>(width) / static_cast<float>(height)
		);
	}

	Camera::~Camera(){}
	
	const Target& Camera::framebuffer() const{
		return _framebuffer;
	}

	Target& Camera::framebuffer(){
		return _framebuffer;
	}

	void Camera::setProjection(Projections::ProjectionType type){
		switch (type){
			case Projections::PERSPECTIVE:{
				_projectionType = Projections::PERSPECTIVE;
				_projection = std::make_unique<Projections::PerspectiveProjection>();
				break;
			}

			case Projections::ORTHOGRAPHIC:{
				_projectionType = Projections::ORTHOGRAPHIC;
				_projection = std::make_unique<Projections::OrthographicProjection>();
				break;
			}
		}
	}

	void Camera::checkProjection(Projections::ProjectionType type){
		if (_projectionType != type) setProjection(type);
	}

	Projections::OrthographicProjection& Camera::orthographic(){
		checkProjection(Projections::ORTHOGRAPHIC);
		return dynamic_cast<Projections::OrthographicProjection&>(*_projection);
	}

	Projections::PerspectiveProjection& Camera::perspective(){
		checkProjection(Projections::PERSPECTIVE);
		return dynamic_cast<Projections::PerspectiveProjection&>(*_projection);
	}

	Projections::ProjectionType Camera::projectionType() const{
		return _projectionType;
	}
	
	const glm::mat4& Camera::viewProjection() const{
		return _projection->viewProjection();
	}

	const glm::mat4& Camera::view() const{
		return _projection->viewMatrix();
	}

	const glm::mat4& Camera::projection() const{
		return _projection->projectionMatrix();
	}
}