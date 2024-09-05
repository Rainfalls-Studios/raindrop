#include "Raindrop/Core/Scenes/Context.hpp"
#include "Raindrop/Core/Context.hpp"

namespace Raindrop::Core::Scenes{
	Context::Context(Core::Context& core) : _core{core}{
		createLogger();
	}

	Context::~Context(){
		
	}

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


	void Context::createLogger(){
		// _logger = _core
	}
}