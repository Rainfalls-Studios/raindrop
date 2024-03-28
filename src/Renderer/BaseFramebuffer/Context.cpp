#include <Raindrop/Renderer/BaseFramebuffer/Context.hpp>

namespace Raindrop::Renderer::BaseFramebuffer{
	Context::Context(Renderer& renderer) :
		renderer{renderer},
		renderPass(*this),
		set(*this),
		framebuffer(*this),
		sampler(*this),
		fullscreenQuad(*this)
	{
		set.update();
	}
}