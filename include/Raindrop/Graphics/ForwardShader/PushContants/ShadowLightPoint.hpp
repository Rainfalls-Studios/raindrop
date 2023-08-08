#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SHADOW_LIGHT_POINT_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SHADOW_LIGHT_POINT_HPP__

#include <Raindrop/Graphics/ForwardShader/PushContants/common.hpp>

namespace Raindrop::Graphics::ForwardShader::PushConstants{
	struct ShadowLightPoint{
		glm::mat4 lightMatrix;
		alignas(16) glm::vec3 cameraPosition;
		alignas(16) glm::vec3 cameraDirection;
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 position;
		float intensity;
	};
}

#endif