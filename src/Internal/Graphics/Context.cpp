#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

#define USE_MAIN_LOGGER false

namespace Raindrop::Internal::Graphics{
	Context::Frame::Frame(Context& context):
		_commandPool{nullptr},
		_queue{nullptr}
	{}	

	Context::Context(Internal::Context& internal) :
	#if USE_MAIN_LOGGER
		_logger{internal.getLogger()},
	#else
		_logger{spdlog::stdout_color_mt("Raindrop::Graphics")},
	#endif
		_internal{internal},
		_allocationCallbacks{nullptr},
		_window(*this),
		_instance(*this),
		_physicalDevice(*this),
		_device(*this),
		_queues(*this),
		_swapchain(*this),
		_memoryTypes(*this),

		frame(*this)
	{}

	Context::~Context(){}

	Internal::Context& Context::getInternalContext() noexcept{
		return _internal;
	}

	VkAllocationCallbacks*& Context::getAllocationCallbacks() noexcept{
		return _allocationCallbacks;
	}

	std::shared_ptr<spdlog::logger> Context::getLogger() noexcept{
		return _logger;
	}

	Core::Window& Context::getWindow() noexcept{
		return _window;
	}

	Core::Instance& Context::getInstance() noexcept{
		return _instance;
	}

	Core::PhysicalDevice& Context::getPhysicalDevice() noexcept{
		return _physicalDevice;
	}

	const Core::PhysicalDevice& Context::getPhysicalDevice() const noexcept{
		return _physicalDevice;
	}

	Core::Device& Context::getDevice() noexcept{
		return _device;
	}

	Queues& Context::getQueues() noexcept{
		return _queues;
	}
	
	Core::Swapchain& Context::getSwapchain() noexcept{
		return _swapchain;
	}

	CommandPool*& Context::Frame::getCommandPool() noexcept{
		return _commandPool;
	}

	Queue*& Context::Frame::getQueue() noexcept{
		return _queue;
	}

	MemoryTypes& Context::getMemoryTypes() noexcept{
		return _memoryTypes;
	}

}