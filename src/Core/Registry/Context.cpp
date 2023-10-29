#include <Raindrop/Core/Registry/Context.hpp>

namespace Raindrop::Core::Registry{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger("Raindrop::Core::Registry")
	{}

	Core::Context& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return _logger;
	}
}