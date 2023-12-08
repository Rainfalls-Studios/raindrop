#include <Raindrop/Graphics/Internal/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Internal{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger(spdlog::stdout_logger_mt("Raindrop::Graphics::Internal"))
	{

		_logger->info("Loading Graphics internal context...");
		_window = std::make_unique<Window>(*this);
		_instance = std::make_unique<Instance>(*this);

		_window->createSurface();

		_physicalDevice = std::make_unique<PhysicalDevice>();
		_device = std::make_unique<Device>(*this);
		_queueHandler = std::make_unique<QueueHandler>(*this);
		_queues = std::make_unique<Queues>(*this);
		_commandPools = std::make_unique<CommandPools>(*this);
		_swapchain = std::make_unique<Swapchain>(*this);
		_logger->info("Graphics internal context loaded without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Graphics internal context...");

		_swapchain.reset();
		_commandPools.reset();
		_queues.reset();
		_queueHandler.reset();
		_device.reset();
		_physicalDevice.reset();
		_window->destroySurface();
		_instance.reset();
		_window.reset();

		_logger->info("Graphics internal context terminated without any critical error");
	}

	const VkPhysicalDeviceLimits& Context::limits() const{
		return _physicalDevice->limits();
	}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	Window& Context::window(){
		return *_window;
	}

	Instance& Context::instance(){
		return *_instance;
	}

	PhysicalDevice& Context::physicalDevice(){
		return *_physicalDevice;
	}

	Device& Context::device(){
		return *_device;
	}

	QueueHandler& Context::queueHandler(){
		return *_queueHandler;
	}

	CommandPools& Context::commandPools(){
		return *_commandPools;
	}

	Swapchain& Context::swapchain(){
		return *_swapchain;
	}

	Queues& Context::queues(){
		return *_queues;
	}
}