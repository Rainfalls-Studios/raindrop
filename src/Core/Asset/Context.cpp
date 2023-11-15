#include <Raindrop/Core/Asset/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Core::Asset{
	Context::Context(Core::Context& core) : 
		_core{core},
		_logger(spdlog::stdout_logger_st("Raindrop::Core::Asset"))
	{}
	
	Core::Context& Context::core(){
		return _core;
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}
}