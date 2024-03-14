#include <Raindrop/Renderer/Scene/Context.hpp>

namespace Raindrop::Renderer::Scene{
	Context::Context(Renderer& renderer) :
		renderer{renderer},
		renderPass(*this),
		framebuffer(*this),
		sampler(*this),
		set(*this)
	{}
}