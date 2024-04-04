#include <Raindrop/Graphics/Materials/Context.hpp>


namespace Raindrop::Graphics::Materials{
	Context::Context(Renderer& renderer) : 
		renderer{renderer},
		instanceCount{POOL_SIZE},
		layout(*this),
		pool(*this),
		buffer(*this),
		registry(*this)
	{}
}