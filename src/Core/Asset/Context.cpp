#include <Raindrop/Core/Asset/Context.hpp>

namespace Raindrop::Core::Asset{
	Context::Context(Core::Context& core) : 
		core{core},
		logger("Core::Asset"){}
}