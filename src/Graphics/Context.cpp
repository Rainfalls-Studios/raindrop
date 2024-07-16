#include <Raindrop/Graphics/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Graphics");
	}

	void Context::initialize(){
		createLogger();

		// None for now
		core.allocationCallbacks = nullptr;

		// First create the window
		window.createLogger();
		window.window.prepare(window);
		window.window.initialize();
		

		// create the core logger
		core.createLogger();
		core.logger->set_level(spdlog::level::trace);

		// Create the vulkan instance
		core.instance.prepare(core);
		core.instance->set_engine_name("Raindrop");
		core.instance->request_validation_layers();
		core.instance.initialize();

		// Create the window surface
		window.surface.prepare(window, core);
		window.surface.initialize();

		// // Require surface support
		core.physicalDevice.prepare(core);
		core.physicalDevice.requireSurfaceSupport(window.surface.get());
		core.physicalDevice.initialize();

		// Create the physical device
		core.device.prepare(core);
		core.device.initialize();

		// Create the window's swapchain
		window.swapchain.prepare(window, core, *this);
		window.swapchain.wantExtent(window.window.getExtent())
						.wantFrameCount(Window::Swapchain::DOUBLE_BUFFERING)
						.wantPresentMode(VK_PRESENT_MODE_FIFO_KHR)
						.wantSurfaceFormat({
							.format = VK_FORMAT_R8G8B8A8_SRGB,
							.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
						});
		window.swapchain.initialize();
	}


	Core::Device& Context::getDevice() noexcept{
		return core.device;
	}

	const Core::Device& Context::getDevice() const noexcept{
		return core.device;
	}

	Core::PhysicalDevice& Context::getPhysicalDevice() noexcept{
		return core.physicalDevice;
	}

	const Core::PhysicalDevice& Context::getPhysicalDevice() const noexcept{
		return core.physicalDevice;
	}

	Core::Instance& Context::getInstance() noexcept{
		return core.instance;
	}

	const Core::Instance& Context::getInstance() const noexcept{
		return core.instance;
	}

}