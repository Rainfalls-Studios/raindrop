#ifndef __RAINDROP_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#include <Raindrop/Core/Context.hpp>
#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Utils/Context.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>
#include <Raindrop/Graphics/RenderPass/Context.hpp>
#include <Raindrop/Graphics/Framebuffer/Context.hpp>
#include <Raindrop/Graphics/Formats/Context.hpp>
#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <Raindrop/Graphics/Loader.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>

namespace Raindrop::Graphics{
	class Context{
		public:
			Context(Core::Engine& core);
			~Context();

			Context(const Context &) = delete;
			Context& operator=(const Context &) = delete;

			Core::Engine& core();
			spdlog::logger& logger();

			VkAllocationCallbacks*& allocationCallbacks();

			Internal::Context& internal();
			Utils::Context& utils();
			Shaders::Context& shaders();
			Image::Context& image();
			RenderPass::Context& renderPass();
			Framebuffer::Context& framebuffer();
			Formats::Context& formats();
			Buffers::Context& buffers();
			Swapchain& swapchain();
			Loader& loader();
		
		private:
			Core::Engine& _core;
			std::shared_ptr<spdlog::logger> _logger;
			
			VkAllocationCallbacks* _allocationCallbacks = nullptr;

			std::unique_ptr<Internal::Context> _internal;
			std::unique_ptr<Utils::Context> _utils;
			std::unique_ptr<Shaders::Context> _shaders;
			std::unique_ptr<Image::Context> _image;
			std::unique_ptr<RenderPass::Context> _renderPass;
			std::unique_ptr<Framebuffer::Context> _framebuffer;
			std::unique_ptr<Formats::Context> _formats;
			std::unique_ptr<Buffers::Context> _buffers;
			std::unique_ptr<Swapchain> _swapchain;
			std::unique_ptr<Loader> _loader;

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