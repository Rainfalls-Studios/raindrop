#include "Raindrop/Core/Context.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Core{
	Context::Context() : 
		_logger{spdlog::stdout_color_st("Raindrop::Core")},
		_assets{std::make_unique<Assets::Context>(*this)},
		_events{std::make_unique<Events::Context>(*this)},
		_scenes{std::make_unique<Scenes::Context>(*this)}
	{}

	Context::~Context(){}

	std::shared_ptr<spdlog::logger>& Context::getLogger() noexcept{
		return _logger;
	}

	const std::shared_ptr<spdlog::logger>& Context::getLogger() const noexcept{
		return _logger;
	}
	
	Assets::Context& Context::getAssets() noexcept{
		return *_assets;
	}

	const Assets::Context& Context::getAssets() const noexcept{
		return *_assets;
	}

	Events::Context& Context::getEvents() noexcept{
		return *_events;
	}

	const Events::Context& Context::getEvents() const noexcept{
		return *_events;
	}

	Scenes::Context& Context::getScenes() noexcept{
		return *_scenes;
	}

	const Scenes::Context& Context::getScenes() const noexcept{
		return *_scenes;
	}

	UUIDRegistry& Context::getRegistry() noexcept{
		return _registry;
	}

	const UUIDRegistry& Context::getRegistry() const noexcept{
		return _registry;
	}
}