#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_PIPELINES_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_PIPELINES_HPP__

#include <Raindrop/Graphics/ForwardShader/Pipelines/common.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/LightPoint.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/Spotlight.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/Sun.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/ShadowLightPoint.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/ShadowSpotlight.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/ShadowSun.hpp>

namespace Raindrop::Graphics::ForwardShader::Pipelines{
	struct Pipelines{
		LightPoint lightPoint;
		Spotlight spotlight;
		Sun sun;
		ShadowLightPoint shadowLightPoint;
		ShadowSpotlight shadowSpotlight;
		ShadowSun shadowSun;

		Pipelines(GraphicsContext& context, VkDescriptorSetLayout setLayout);
	};
}

#endif