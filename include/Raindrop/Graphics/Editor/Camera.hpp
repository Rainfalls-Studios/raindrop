#ifndef __RAINDROP_GRAPHICS_EDITOR_CAMERA_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_CAMERA_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class Camera{
		public:
			enum Type{
				PERSPECTIVE,
				ORTHOGRAPHIC,
			};

			glm::mat4 projection = glm::mat4(1.f);
			glm::mat4 view = glm::mat4(1.f);
			glm::mat4 viewProjection = glm::mat4(1.f);

			glm::vec3 translation = glm::vec3(0, 0, 0);
			glm::vec3 rotation = glm::vec3(0, 0, 0);

			glm::vec3 rotationSensivity = glm::vec3(0.001f);
			glm::vec3 translationSensivity = glm::vec3(0.01f);

			/**
			 * @note in a perspective camera, the apsect ratio is based of abs(left + right) / abs(top + down)
			 *
			 */
			glm::vec2 width = glm::vec2(-1, 1); // [LEFT | RIGHT]
			glm::vec2 height = glm::vec2(-1, 1); // [TOP | DOWN]
			glm::vec2 depth = glm::vec2(0.01f, 1000.f); // [NEAR | FAR]

			glm::vec3 forward = glm::vec3(0.f);
			glm::vec3 up = glm::vec3(0.f);
			glm::vec3 right = glm::vec3(0.f);

			// only used in perspective mode
			float fov = 75.f;

			Type type = PERSPECTIVE;

			void update();
	};
}

#endif