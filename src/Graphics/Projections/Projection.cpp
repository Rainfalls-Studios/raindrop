// #include <Raindrop/Graphics/Projections/Projection.hpp>

// namespace Raindrop::Graphics::Projections{
// 	const glm::mat4& Projection::viewMatrix() const{
// 		return _viewMatrix;
// 	}

// 	const glm::mat4& Projection::projectionMatrix() const{
// 		return _projectionMatrix;
// 	}

// 	const glm::mat4& Projection::viewProjection() const{
// 		return _viewProjectionMatrix;
// 	}
	
	
// 	void Projection::setPosition(const glm::vec3& position){
// 		_position = position;
// 		update();
// 	}

// 	void Projection::setRotation(const glm::quat& rotation){
// 		_rotation = rotation;
// 		update();
// 	}
	
// 	const glm::vec3& Projection::position() const{
// 		return _position;
// 	}

// 	const glm::quat& Projection::rotation() const{
// 		return _rotation;
// 	}
	
// 	void Projection::setTransform(const glm::vec3& position, const glm::quat& rotation){
// 		_position = position;
// 		_rotation = rotation;
// 		update();
// 	}
// }