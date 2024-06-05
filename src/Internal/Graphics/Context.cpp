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

	Internal::Context& Context::getInternalContext(){
		return _internal;
	}

	VkAllocationCallbacks*& Context::getAllocationCallbacks(){
		return _allocationCallbacks;
	}

	std::shared_ptr<spdlog::logger> Context::getLogger(){
		return _logger;
	}

	Core::Window& Context::getWindow(){
		return _window;
	}

	Core::Instance& Context::getInstance(){
		return _instance;
	}

	Core::PhysicalDevice& Context::getPhysicalDevice(){
		return _physicalDevice;
	}

	Core::Device& Context::getDevice(){
		return _device;
	}

	Queues& Context::getQueues(){
		return _queues;
	}
	
	Core::Swapchain& Context::getSwapchain(){
		return _swapchain;
	}

	CommandPool*& Context::Frame::getCommandPool(){
		return _commandPool;
	}

	Queue*& Context::Frame::getQueue(){
		return _queue;
	}

	MemoryTypes& Context::getMemoryTypes(){
		return _memoryTypes;
	}

}