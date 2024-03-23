#include <Raindrop/Renderer/Material/Context.hpp>


namespace Raindrop::Renderer::Material{
	Context::Context(Renderer& renderer) : 
		renderer{renderer},
		instanceCount{POOL_SIZE},
		layout(*this),
		pool(*this),
		buffer(*this),
		registry(*this)
	{}
}