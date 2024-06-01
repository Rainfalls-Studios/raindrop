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
		logger{internal.getLogger()},
	#else
		logger{spdlog::stdout_color_mt("Raindrop::Graphics")},
	#endif
		internal{internal},
		allocationCallbacks{nullptr},
		window(*this),
		instance(*this),
		physicalDevice(*this),
		device(*this),
		queues(*this),
		swapchain(*this),

		frame(*this)
	{}

	Context::~Context(){}

	Internal::Context& Context::getInternalContext(){
		return internal;
	}

	VkAllocationCallbacks*& Context::getAllocationCallbacks(){
		return allocationCallbacks;
	}

	std::shared_ptr<spdlog::logger> Context::getLogger(){
		return logger;
	}

	Core::Window& Context::getWindow(){
		return window;
	}

	Core::Instance& Context::getInstance(){
		return instance;
	}

	Core::PhysicalDevice& Context::getPhysicalDevice(){
		return physicalDevice;
	}

	Core::Device& Context::getDevice(){
		return device;
	}

	Queues& Context::getQueues(){
		return queues;
	}
	
	Core::Swapchain& Context::getSwapchain(){
		return swapchain;
	}

	CommandPool*& Context::Frame::getCommandPool(){
		return _commandPool;
	}

	Queue*& Context::Frame::getQueue(){
		return _queue;
	}
}