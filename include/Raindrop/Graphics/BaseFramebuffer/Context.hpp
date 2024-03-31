#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_CONTEXT_HPP__

#include "common.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Sampler.hpp"
#include "DescriptorSet.hpp"
#include "FullscreenQuad.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	struct Context{
		using Renderer = ::Raindrop::Graphics::Context;
		Context(Renderer& renderer);

		Renderer& renderer;

		RenderPass renderPass;
		DescriptorSet set;
		Sampler sampler;
		FullscreenQuad fullscreenQuad;
	};
}

#endif