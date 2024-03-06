#include <Raindrop/Context.hpp>

namespace Raindrop{
	Context::Context() : 
		running{false},
		assetManager(),
		eventManager(),
		renderer(*this)
	{}
}