#ifndef __RAINDROP_INTERNAL_RENDER_PASS_HPP__
#define __RAINDROP_INTERNAL_RENDER_PASS_HPP__

#include <Raindrop/RenderPass.hpp>
#include <Raindrop_internal/Graphics/RenderPass.hpp>

namespace Raindrop{
	struct RenderPass::Impl{
		std::shared_ptr<Internal::Graphics::RenderPass> renderPass;
		Internal::Graphics::RenderPassConfigInfo info;
		Context* context;

		Impl(Context& context) : context{&context}{}
		Impl() : context{nullptr}{}
	};
}

#endif