#include <Raindrop/Renderer/Scene/Context.hpp>

namespace Raindrop::Renderer::Scene{
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