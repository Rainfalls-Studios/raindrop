#include <Raindrop/Graphics/Projections/OrthographicProjection.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Raindrop::Graphics::Projections{
	OrthographicProjection::OrthographicProjection(){
		setProjection(1.f, 1.f, 1.f);
	}


	void OrthographicProjection::setProjection(float width, float height, float depth){
		_width = width;
		_height = _height;
		_depth = _depth;

		_projectionMatrix = glm::ortho(
			-_width,
			 _width,
			-_height,
			 _height,
			 0.f,
			 _depth
		);

		update();
	}

	void OrthographicProjection::update(){
		glm::mat4 rotationMatrix = glm::mat4_cast(_rotation);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);

		glm::mat4 transform = translationMatrix * rotationMatrix;
		_viewMatrix = glm::inverse(transform);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}
}