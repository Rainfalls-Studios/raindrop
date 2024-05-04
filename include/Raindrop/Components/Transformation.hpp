#ifndef __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__
#define __RAINDROP_COMPONENTS_TRANSFORMATION_HPP__

#include "common.hpp"

namespace Raindrop::Components{
	class Transformation{
		public:
			Transformation();

			inline void setMatrix(const Maths::mat4& matrix);
			inline Maths::mat4& getMatrix();
			inline const Maths::mat4& getMatrix() const;

			inline void setTranslation(const Maths::vec3& translation);
			inline Maths::vec3& getTranslation();
			inline const Maths::vec3& getTranslation() const;

			inline void setScale(const Maths::vec3& scale);
			inline Maths::vec3& getScale();
			inline const Maths::vec3& getScale() const;
			
			inline void setRotation(const Maths::quat& rotation);
			inline Maths::quat& getRotation();
			inline const Maths::quat& getRotation() const;

			void updateMatrix();

		private:
			Maths::mat4 _matrix;
			Maths::vec3 _translation;
			Maths::vec3 _scale;
			Maths::quat _rotation;
	};
}

#include "Transformation.inl"

#endif