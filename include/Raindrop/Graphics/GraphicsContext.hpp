#ifndef __RAINDROP_GRAPHICS_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_CONTEXT_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Swapchain.hpp>
#include <Raindrop/Graphics/TransfertCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsRegistry.hpp>

#include <Raindrop/Graphics/RenderPasses.hpp>
#include <Raindrop/Graphics/DescriptorSetLayouts.hpp>
#include <Raindrop/Graphics/GraphicsFamily.hpp>
#include <Raindrop/Graphics/TransfertFamily.hpp>
#include <Raindrop/Graphics/PresentFamily.hpp>
#include <Raindrop/Graphics/GlobalDescriptorPool.hpp>
#include <Raindrop/Graphics/DummyTexture.hpp>

namespace Raindrop::Graphics{
	struct GraphicsContext{
		GraphicsContext(Core::EngineContext& context);
		~GraphicsContext();

		GraphicsContext(const GraphicsContext &) = delete;
		GraphicsContext& operator=(const GraphicsContext &) = delete;
		
		Core::EngineContext& context;

		GraphicsRegistry gRegistry;
		VkAllocationCallbacks* allocationCallbacks = nullptr;

		Window window;
		Instance instance;
		Device device;
		Swapchain swapchain;

		GlobalDescriptorPool descriptorPool;
		VkRenderPass sceneRenderPass;

		DescriptorLayouts layouts;
		RenderPasses renderPasses;

		GraphicsFamily graphics;
		TransfertFamily transfert;
		PresentFamily present;

		DummyTexture dummyTexture;
	};
}

#endif