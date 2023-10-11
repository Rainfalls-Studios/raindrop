#include <Raindrop/Graphics/Utils/Context.hpp>

namespace Raindrop::Graphics::Utils{
	Context::Context(Graphics::Context& graphics) :
		graphics{graphics},
		logger(graphics.logger.name() + "::Utils"),
		formats(*this){}
}