#include "Raindrop/Core/Assets/Context.hpp"
#include "Raindrop/Core/Context.hpp"

namespace Raindrop::Core::Assets{
	Context::Context(Core::Context& core) : 
		_core{core},
		_logger{core.getLogger()->clone("Raindrop::Core::Assets")},
		_manager{*this}
	{}

	Context::~Context(){}
	
	Core::Context& Context::getCore() noexcept{
		return _core;
	}

	const Core::Context& Context::getCore() const noexcept{
		return _core;
	}

	std::shared_ptr<spdlog::logger>& Context::getLogger() noexcept{
		return _logger;
	}

	const std::shared_ptr<spdlog::logger>& Context::getLogger() const noexcept{
		return _logger;
	}

	Manager& Context::getManager() noexcept{
		return _manager;
	}

	const Manager& Context::getManager() const noexcept{
		return _manager;
	}
}