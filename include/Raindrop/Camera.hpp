#ifndef __RAINDROP_CAMERA_HPP__
#define __RAINDROP_CAMERA_HPP__

#include "common.hpp"
#include "Components/Transformation.hpp"

namespace Raindrop{
	class Camera{
		public:
			Camera();
			~Camera();

			void translate(const Maths::vec3& translation);
			void scale(const Maths::vec3& factor);
			void rotate(const Maths::quat& rotation);

			void setTranslation(const Maths::vec3& translation);
			void setScale(const Maths::vec4& scale);
			void setRotation(const Maths::quat& rotation);

			void setAspectRatio(float ratio);
			void setFov(float fov);

			const Maths::mat4& projection() const;
			const Maths::mat4& transform() const;
			const Maths::mat4& viewProjection() const;

			const Maths::vec3& translation() const;
			const Maths::vec3& scale() const;
			const Maths::quat& rotation() const;

			void updateTransform();
			void updateProjection();
			void updateViewProjection();
			
			
		private:
			Maths::mat4 _projection;
			Maths::mat4 _viewProjection;

			Components::Transformation _transform;

			float _aspectRatio;
			float _fov;

	};
}

#endif