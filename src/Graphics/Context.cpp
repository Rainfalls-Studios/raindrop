#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Engine.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics{
	Context::Context(Core::Engine& core) :
		_core{core},
		_logger(spdlog::stdout_logger_mt("Raindrop::Graphics")),
		_registry{_core.registry().add_child("Graphics", {})}{
			
		_logger->info("Loading Graphics context...");
		_internal = std::make_unique<Internal::Context>(*this);
		_utils = std::make_unique<Utils::Context>(*this);
		_shaders = std::make_unique<Shaders::Context>(*this);
		_logger->info("Graphics context loaded without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Graphics context...");
		_shaders.reset();
		_utils.reset();
		_internal.reset();
		_logger->info("Graphics context terminated without any critical error");
	}

	Core::Engine& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	Core::Registry::Registry& Context::registry(){
		return _registry;
	}

	VkAllocationCallbacks*& Context::allocationCallbacks(){
		return _allocationCallbacks;
	}

	Internal::Context& Context::internal(){
		return *_internal;
	}

	Utils::Context& Context::utils(){
		return *_utils;
	}

	Shaders::Context& Context::shaders(){
		return *_shaders;
	}
}