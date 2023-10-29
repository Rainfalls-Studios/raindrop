#include <Raindrop/Core/Scene/Context.hpp>

namespace Raindrop::Core::Scene{
	Context::Context(Core::Context& core) : 
		_core{core},
		_logger("Raindrop::Core::Scene")
	{}
	
	Core::Context& Context::core(){
		return _core;
	}
	
	spdlog::logger& Context::logger(){
		return _logger;
	}
}