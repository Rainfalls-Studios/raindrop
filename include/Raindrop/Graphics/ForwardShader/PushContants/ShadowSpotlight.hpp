#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SHADOW_SPOTLIGHT_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PUSH_CONSTANTS_SHADOW_SPOTLIGHT_HPP__

#include <Raindrop/Graphics/ForwardShader/PushContants/common.hpp>

namespace Raindrop::Graphics::ForwardShader::PushConstants{
	struct ShadowSpotlight{
		glm::mat4 lightMatrix;
		glm::vec3 cameraPosition;
		glm::vec3 cameraDirection;
		glm::vec3 position;
		glm::vec3 direction;
		uint32_t color;
		float outerCutOff;
		float cutOff;
		float intensity;
	};
}

#endif