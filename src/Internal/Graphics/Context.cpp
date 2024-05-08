#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>

#include <spdlog/sinks/stdout_color_sinks.h>

#define USE_MAIN_LOGGER false

namespace Raindrop::Internal::Graphics{
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
		commandPools(*this),
		swapchain(*this)
		// pipelineLayoutRegistry(*this),
		// graphicsPipelineRegistry(*this)
	{}

	Internal::Context& Context::getInternalContext(){
		return internal;
	}

	VkAllocationCallbacks*& Context::getAllocationCallbacks(){
		return allocationCallbacks;
	}

	spdlog::logger& Context::getLogger(){
		return *logger;
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

	Queues::Manager& Context::getQueues(){
		return queues;
	}

	Queues::CommandPools& Context::getCommandPools(){
		return commandPools;
	}

	Core::Swapchain& Context::getSwapchain(){
		return swapchain;
	}
}