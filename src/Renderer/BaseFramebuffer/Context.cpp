#include <Raindrop/Graphics/BaseFramebuffer/Context.hpp>

namespace Raindrop::Graphics::BaseFramebuffer{
	Context::Context(Renderer& renderer) :
		renderer{renderer},
		renderPass(*this),
		set(*this),
		sampler(*this),
		fullscreenQuad(*this){}
}