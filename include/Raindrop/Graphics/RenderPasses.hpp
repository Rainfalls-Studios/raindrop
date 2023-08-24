#ifndef __RAINDROP_GRAPHICS_RENDER_PASSES_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASSES_HPP__

#include <Raindrop/Graphics/common.hpp>

#include <Raindrop/Graphics/ShadowMap/Sun/RenderPass.hpp>
#include <Raindrop/Graphics/ForwardShader/RenderPass.hpp>
#include <Raindrop/Graphics/SceneRenderPass.hpp>

namespace Raindrop::Graphics{
	struct RenderPasses{
		RenderPasses(GraphicsContext& context);
		~RenderPasses();

		ShadowMap::Sun::RenderPass sun;
		ForwardShader::RenderPass forwardShader;
		SceneRenderPass scene;
	};
}

#endif