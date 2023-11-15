#include <Raindrop/Core/Scene/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Core::Scene{
	Context::Context(Core::Context& core) : 
		_core{core},
		_logger(spdlog::stdout_logger_mt("Raindrop::Core::Scene"))
	{}
	
	Core::Context& Context::core(){
		return _core;
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}
}