#ifndef __RAINDROP_CAMERA_HPP__
#define __RAINDROP_CAMERA_HPP__

#include "common.hpp"
#include "Components/Transformation.hpp"

namespace Raindrop{
	class Camera{
		public:
			Camera();
			~Camera();

			void translate(const glm::vec3& translation);
			void scale(const glm::vec3& factor);
			void rotate(const glm::quat& rotation);

			void setTranslation(const glm::vec3& translation);
			void setScale(const glm::vec4& scale);
			void setRotation(const glm::quat& rotation);

			void setAspectRatio(float ratio);
			void setFov(float fov);

			const glm::mat4& projection() const;
			const glm::mat4& transform() const;
			const glm::mat4& viewTransform() const;

			const glm::vec3& translation() const;
			const glm::vec3& scale() const;
			const glm::quat& rotation() const;

			void updateTransform();
			void updateProjection();
			void updateViewTransform();
			
		private:
			glm::mat4 _projection;
			glm::mat4 _viewTranform;

			Components::Transformation _transform;

			float _aspectRatio;
			float _fov;

	};
}

#endif