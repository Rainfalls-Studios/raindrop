#include <Raindrop/Core/Scene/Context.hpp>

namespace Raindrop::Core::Scene{
	Context::Context(Core::Context& core) : 
		core{core},
		logger("Core::Scene"){}
}