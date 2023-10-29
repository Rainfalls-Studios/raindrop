#include <Raindrop/Core/Asset/Context.hpp>

namespace Raindrop::Core::Asset{
	Context::Context(Core::Context& core) : 
		_core{core},
		_logger("Raindrop::Core::Asset")
	{}
	
	Core::Context& Context::core(){
		return _core;
	}
	
	spdlog::logger& Context::logger(){
		return _logger;
	}
}