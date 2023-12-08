#include <Raindrop/Core/Registry/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Core::Registry{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger(spdlog::stdout_logger_mt("Raindrop::Core::Registry"))
	{
		_registry = std::make_unique<Registry>(*this);
	}

	Context::~Context(){
		_registry.reset();
	}

	Core::Context& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	Registry& Context::registry(){
		return *_registry;
	}

}