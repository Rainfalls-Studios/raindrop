#include <Raindrop/Graphics/Internal/Context.hpp>

namespace Raindrop::Graphics::Internal{
	Context::Context(Graphics::Context& graphics) : 
		graphics{graphics},
		logger("Graphics::Internal"),
		window(*this),
		instance(*this){}
}