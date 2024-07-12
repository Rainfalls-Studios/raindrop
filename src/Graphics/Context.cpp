#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	void Context::initialize(){

		// None for now
		core.allocationCallbacks = nullptr;

		// First create the window
		window.createLogger();
		window.window.initialize(window);
		

		// Then create the vulkan instance
		core.createLogger();
		core.logger->set_level(spdlog::level::trace);

		// Require the window mendatory extensions
		core.instance.requireExtensions(window.window.getRequiredInstanceExtensions());

		// Create the instance
		core.instance.initialize(core);


		// Create the window surface
		window.surface.initialize(window, core);


		// Require surface support
		core.physicalDevice.requireSurfaceSupport(window.surface.get());

		// Find a suitable physical device
		core.physicalDevice.initialize(core);


		// Require swapchain support for the window
		core.device.requireExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	
		core.device.initialize(core);

		// Initialize syncronization
		sync.createLogger();
		sync.manager.initialize(sync, core);
	}
}