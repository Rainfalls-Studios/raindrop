#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics{
	Context::Context(Core::Engine& core) :
		_core{core},
		_logger("Raindrop::Graphics"),
		_registry{_core.registry().add_child("Graphics", {})},
		_internal(*this),
		_utils(*this)
	{}

	Context::~Context(){}

	Core::Engine& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return _logger;
	}

	Core::Registry::Registry& Context::registry(){
		return _registry;
	}

	VkAllocationCallbacks*& Context::allocationCallbacks(){
		return _allocationCallbacks;
	}

	Internal::Context& Context::internal(){
		return _internal;
	}

	Utils::Context& Context::utils(){
		return _utils;
	}
}