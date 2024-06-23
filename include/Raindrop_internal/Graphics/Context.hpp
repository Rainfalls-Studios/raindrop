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

			Internal::Context& getInternalContext() noexcept;
			VkAllocationCallbacks*& getAllocationCallbacks() noexcept;
			std::shared_ptr<spdlog::logger> getLogger() noexcept;

			Core::Window& getWindow() noexcept;
			Core::Instance& getInstance() noexcept;
			Core::PhysicalDevice& getPhysicalDevice() noexcept;
			const Core::PhysicalDevice& getPhysicalDevice() const noexcept;
			Core::Device& getDevice() noexcept;
			Queues& getQueues() noexcept;
			Core::Swapchain& getSwapchain() noexcept;
			MemoryTypes& getMemoryTypes() noexcept;

			struct Frame{
				public:
					Frame(Context& context);

					CommandPool*& getCommandPool() noexcept;
					Queue*& getQueue() noexcept;
				
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