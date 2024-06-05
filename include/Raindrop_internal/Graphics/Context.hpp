#ifndef __RAINDROP_INTERNAL_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Instance.hpp"
#include "Core/Device.hpp"
#include "Core/PhysicalDevice.hpp"
#include "Core/Window.hpp"
#include "Core/Swapchain.hpp"
#include "Queues.hpp"
#include "CommandPool.hpp"
#include "Memory.hpp"

namespace Raindrop::Internal::Graphics{
	class Context{
		public:
			Context(Internal::Context& internal);
			~Context();

			Internal::Context& getInternalContext();
			VkAllocationCallbacks*& getAllocationCallbacks();
			std::shared_ptr<spdlog::logger> getLogger();

			Core::Window& getWindow();
			Core::Instance& getInstance();
			Core::PhysicalDevice& getPhysicalDevice();
			Core::Device& getDevice();
			Queues& getQueues();
			Core::Swapchain& getSwapchain();
			MemoryTypes& getMemoryTypes();

			struct Frame{
				public:
					Frame(Context& context);

					CommandPool*& getCommandPool();
					Queue*& getQueue();
				
				private:
					CommandPool* _commandPool;
					Queue* _queue;
			} frame;

		private:
			Internal::Context& _internal;
			VkAllocationCallbacks* _allocationCallbacks;
			std::shared_ptr<spdlog::logger> _logger;

			Core::Window _window;
			Core::Instance _instance;
			Core::PhysicalDevice _physicalDevice;
			Core::Device _device;
			Queues _queues;
			Core::Swapchain _swapchain;
			MemoryTypes _memoryTypes;
	};
}

#endif