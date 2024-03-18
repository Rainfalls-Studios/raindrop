#include <Raindrop/Context.hpp>

namespace Raindrop{
	Context::Context() : 
		running{false},
		assetManager(*this),
		eventManager(),
		renderer(*this)
	{}
}