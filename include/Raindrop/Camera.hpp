#ifndef __RAINDROP_CAMERA_HPP__
#define __RAINDROP_CAMERA_HPP__

#include "common.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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


		private:
			glm::mat4 _projection;
			glm::mat4 _transform;
			glm::mat4 _viewTranform;

			glm::vec3 _translation;
			glm::vec3 _scale;
			glm::quat _rotation;

			float _aspectRatio;
			float _fov;

			void updateTransform();
			void updateProjection();
			void updateViewTransform();
	};
}

#endif