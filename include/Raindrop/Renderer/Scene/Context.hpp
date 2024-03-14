#ifndef __RAINDROP_RENDERER_SCENE_CONTEXT_HPP__
#define __RAINDROP_RENDERER_SCENE_CONTEXT_HPP__

#include "common.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Sampler.hpp"
#include "DescriptorSet.hpp"

namespace Raindrop::Renderer::Scene{
	struct Context{
		using Renderer = ::Raindrop::Renderer::Context;
		Context(Renderer& renderer);

		Renderer& renderer;

		RenderPass renderPass;
		Framebuffer framebuffer;
		Sampler sampler;
		DescriptorSet set;
	};
}

#endif