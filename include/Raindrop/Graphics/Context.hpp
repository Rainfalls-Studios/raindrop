#ifndef __RAINDROP_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#include <Raindrop/Core/Context.hpp>
#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Utils/Context.hpp>
// #include <Raindrop/Core/Registry/Registry.hpp>
// #include <Raindrop/Graphics/GlobalDescriptorPool.hpp>
// #include <Raindrop/Graphics/RenderPassesManager.hpp>

// #include <Raindrop/Graphics/Internal/Instance.hpp>
// #include <Raindrop/Graphics/Internal/Window.hpp>
// #include <Raindrop/Graphics/Internal/Device.hpp>
// #include <Raindrop/Graphics/Internal/PhysicalDevice.hpp>
// #include <Raindrop/Graphics/Internal/Swapchain.hpp>

// #include <Raindrop/Graphics/Utils/FormatUtilities.hpp>
// #include <Raindrop/Graphics/Utils/stringToVulkan.hpp>

// #include <Raindrop/Graphics/Queues/PresentFamily.hpp>
// #include <Raindrop/Graphics/Queues/GraphicsFamily.hpp>
// #include <Raindrop/Graphics/Queues/TransfertFamily.hpp>

namespace Raindrop::Graphics{
	class Context{
		public:
			Context(Core::Engine& core);
			~Context();

			Context(const Context &) = delete;
			Context& operator=(const Context &) = delete;

			Core::Engine& core();
			spdlog::logger& logger();

			Core::Registry::Node& registry();
			VkAllocationCallbacks*& allocationCallbacks();

			Internal::Context& internal();
			Utils::Context& utils();
		
		private:
			Core::Engine& _core;
			spdlog::logger _logger;

			Core::Registry::Node _registry;
			VkAllocationCallbacks* _allocationCallbacks = nullptr;

			Internal::Context _internal;
			Utils::Context _utils;

		// GlobalDescriptorPool descriptorPool;
		// // Shaders::ShaderCompiler shaderCompiler;
		// RenderPassesManager renderPasses;

		// // DescriptorLayouts layouts;

		// Queues::GraphicsFamily graphics;
		// Queues::TransfertFamily transfert;
		// Queues::PresentFamily present;
	};
}

#endif