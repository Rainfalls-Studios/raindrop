#ifndef __RAINDROP_INTERNAL_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Instance.hpp"
#include "Core/Device.hpp"
#include "Core/PhysicalDevice.hpp"
#include "Core/Window.hpp"
#include "Queues/Manager.hpp"
#include "Core/Swapchain.hpp"
#include "Queues/CommandPools.hpp"
// #include "Pipelines/LayoutRegistry.hpp"
// #include "Pipelines/GraphicsPipelineRegistry.hpp"


namespace Raindrop::Internal::Graphics{
	class Context{
		public:
			Context(Internal::Context& internal);
			~Context() = default;

			Internal::Context& getInternalContext();
			VkAllocationCallbacks*& getAllocationCallbacks();
			std::shared_ptr<spdlog::logger> getLogger();

			Core::Window& getWindow();
			Core::Instance& getInstance();
			Core::PhysicalDevice& getPhysicalDevice();
			Core::Device& getDevice();
			Queues::Manager& getQueues();
			Queues::CommandPools& getCommandPools();
			Core::Swapchain& getSwapchain();

		private:
			Internal::Context& internal;
			VkAllocationCallbacks* allocationCallbacks;
			std::shared_ptr<spdlog::logger> logger;

			Core::Window window;
			Core::Instance instance;
			Core::PhysicalDevice physicalDevice;
			Core::Device device;
			Queues::Manager queues;
			Queues::CommandPools commandPools;
			Core::Swapchain swapchain;
		// Pipelines::LayoutRegistry pipelineLayoutRegistry;
		// Pipelines::GraphicsPipelineRegistry graphicsPipelineRegistry;
	};
}

#endif