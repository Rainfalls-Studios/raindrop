#include "Raindrop/Graphics/Context.hpp"
#include "Raindrop/Core/Context.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger{spdlog::stdout_color_st("Raindrop::Graphics")},
		_window(*this, "Raindrop", 1080, 720),
		_vkInstance(*this, _window.vkExtensions()),
		_vkSurface(*this, _window.createSurface()),
		_vkPhysicalDevice(*this),
		_vkDevice(*this),
		_vkSwapchain(*this)
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

	Vulkan::PhysicalDevice& Context::getPhysicalDevice() noexcept{
		return _vkPhysicalDevice;
	}

	const Vulkan::PhysicalDevice& Context::getPhysicalDevice() const noexcept{
		return _vkPhysicalDevice;
	}

	Vulkan::Device& Context::getDevice() noexcept{
		return _vkDevice;
	}

	const Vulkan::Device& Context::getDevice() const noexcept{
		return _vkDevice;
	}

	Core::UUIDRegistry& Context::getRegistry() noexcept{
		return _core.getRegistry();
	}

	const Core::UUIDRegistry& Context::getRegistry() const noexcept{
		return _core.getRegistry();
	}

	Vulkan::Swapchain& Context::getSwapchain() noexcept{
		return _vkSwapchain;
	}

	const Vulkan::Swapchain& Context::getSwapchain() const noexcept{
		return _vkSwapchain;
	}
}