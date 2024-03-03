#include <Raindrop/Context.hpp>

namespace Raindrop{
	Context::Context() : 
		assetManager(),
		eventManager(),
		renderer(*this)
	{}
}