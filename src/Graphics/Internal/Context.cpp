#include <Raindrop/Graphics/Internal/Context.hpp>

namespace Raindrop::Graphics::Internal{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger("Raindrop::Graphics::Internal"),
		_window(*this),
		_instance(*this),
		_device(*this),
		_queueHandler(*this),
		_commandPools(*this),
		_swapchain(*this)
	{}

	const VkPhysicalDeviceLimits& Context::limits() const{
		return _physicalDevice.limits();
	}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return _logger;
	}

	Window& Context::window(){
		return _window;
	}

	Instance& Context::instance(){
		return _instance;
	}

	PhysicalDevice& Context::physicalDevice(){
		return _physicalDevice;
	}

	Device& Context::device(){
		return _device;
	}

	QueueHandler& Context::queueHandler(){
		return _queueHandler;
	}

	CommandPools& Context::commandPools(){
		return _commandPools;
	}

	Swapchain& Context::swapchain(){
		return _swapchain;
	}
}