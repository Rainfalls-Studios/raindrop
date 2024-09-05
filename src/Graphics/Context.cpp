#include "Raindrop/Graphics/Context.hpp"
#include "Raindrop/Core/Context.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger{spdlog::stdout_color_st("Raindrop::Graphics")},
		_window(*this, "Raindrop", 1080, 720),
		_vkInstance(*this, _window.vkExtensions()),
		_vkSurface{*this, _window.createSurface()}
	{}

	Context::~Context(){

	}

	std::shared_ptr<spdlog::logger>& Context::getLogger() noexcept{
		return _logger;
	}

	const std::shared_ptr<spdlog::logger>& Context::getLogger() const noexcept{
		return _logger;
	}

	Window& Context::getWindow() noexcept{
		return _window;
	}

	const Window& Context::getWindow() const noexcept{
		return _window;
	}

	Vulkan::Instance& Context::getInstance() noexcept{
		return _vkInstance;
	}

	const Vulkan::Instance& Context::getInstance() const noexcept{
		return _vkInstance;
	}

	Vulkan::Surface& Context::getSurface() noexcept{
		return _vkSurface;
	}

	const Vulkan::Surface& Context::getSurface() const noexcept{
		return _vkSurface;
	}
}