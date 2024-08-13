#include <Raindrop/Graphics/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <Raindrop/Context.hpp>

#include <Raindrop/Graphics/ShaderModuleFactory.hpp>

namespace Raindrop::Graphics{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Graphics");
	}

	void Context::initialize(Raindrop::Context& raindrop){
		this->raindrop = &raindrop;
		createLogger();

		// None for now
		core.allocationCallbacks = nullptr;

		// First create the window
		window.createLogger();
		window.window.prepare(window, *raindrop.events.get());
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
		window.surface.prepare(window, core)
		              .initialize();

		// Require surface support
		core.physicalDevice.prepare(core)
						   .requireSurfaceSupport(window.surface.get())
						   .initialize();
		
		// Initialize the format
		formats.prepare(*this)
		       .initialize();

		// Create the physical device
		core.device.prepare(core)
				   .initialize();


		// Create the window's swapchain
		window.swapchain.prepare(window, core, *this)
						.wantExtent(window.window.getExtent())
						.wantFrameCount(Window::Swapchain::DOUBLE_BUFFERING)
						.wantPresentMode(VK_PRESENT_MODE_FIFO_KHR)
						.wantSurfaceFormat({
							.format = VK_FORMAT_R8G8B8A8_SRGB,
							.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
						})
						.initialize();

		transfertCommandPool.prepare(*this)
			.setCreateFlags(VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
			.setQueueFamily(core.device.transfetQueue.familyIndex)
			.initialize();
		
		modelLoader.prepare(*this)
			.initialize();
		

		createAssetFactories();
	}

	void Context::createAssetFactories(){
		auto& assetManager = raindrop->assets->manager;

		assetManager.emplaceFactory<ShaderModuleFactory>(*this);
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