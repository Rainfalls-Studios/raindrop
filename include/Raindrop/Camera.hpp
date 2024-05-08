// #ifndef __RAINDROP_CAMERA_HPP__
// #define __RAINDROP_CAMERA_HPP__

// #include "common.hpp"
// #include "Components/Transformation.hpp"

// namespace Raindrop{
// 	class Camera{
// 		public:
// 			Camera();
// 			~Camera();

// 			inline void translate(const Maths::vec3& translation);
// 			inline void scale(const Maths::vec3& factor);
// 			inline void rotate(const Maths::quat& rotation);

// 			inline void setTranslation(const Maths::vec3& translation);
// 			inline void setScale(const Maths::vec4& scale);
// 			inline void setRotation(const Maths::quat& rotation);

// 			inline void setAspectRatio(float ratio);
// 			inline void setFov(float fov);

// 			inline const Maths::mat4& getProjection() const;
// 			inline const Maths::mat4& getTransform() const;
// 			inline const Maths::mat4& getViewProjection() const;

// 			inline const Maths::vec3& getTranslation() const;
// 			inline const Maths::vec3& getScale() const;
// 			inline const Maths::quat& getRotation() const;

// 			void updateTransform();
// 			void updateProjection();
// 			void updateViewProjection();
			
// 		private:
// 			Maths::mat4 _projection;
// 			Maths::mat4 _viewProjection;

// 			Components::Transformation _transform;

// 			float _aspectRatio;
// 			float _fov;

// 	};
// }

// #include "Camera.inl"

// #endif