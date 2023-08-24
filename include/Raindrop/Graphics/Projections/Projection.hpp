#ifndef __RAINDROP_GRAPHICS_PROJECTIONS_PROJECTION_HPP__
#define __RAINDROP_GRAPHICS_PROJECTIONS_PROJECTION_HPP__

#include <Raindrop/Graphics/Projections/common.hpp>

namespace Raindrop::Graphics::Projections{
	class Projection{
		public:
			virtual ~Projection() = default;

			const glm::mat4& viewMatrix() const;
			const glm::mat4& projectionMatrix() const;
			const glm::mat4& viewProjection() const;

			void setPosition(const glm::vec3& position);
			void setRotation(const glm::quat& rotation);

			void setTransform(const glm::vec3& position, const glm::quat& rotation);

			const glm::vec3& position() const;
			const glm::quat& rotation() const;

		protected:
			glm::mat4 _viewMatrix;
			glm::mat4 _projectionMatrix;
			glm::mat4 _viewProjectionMatrix;

			glm::vec3 _position;
			glm::quat _rotation;
			
			virtual void update() = 0;
	};
}

#endif