#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Core{
	Context::Context() : 
		logger("Core"),
		registry(*this),
		eventManager(*this),
		assetManager(*this),
		sceneManager(*this)
	{}
}