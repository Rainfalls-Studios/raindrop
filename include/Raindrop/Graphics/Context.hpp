#ifndef __RAINDROP_GRAPHICS_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CONTEXT_HPP__

// #include "../Context.hpp"
#include "common.hpp"
#include "Core/Instance.hpp"
#include "Core/Device.hpp"
#include "Core/PhysicalDevice.hpp"
#include "Core/Window.hpp"
#include "Queues/Manager.hpp"
#include "Core/Swapchain.hpp"
#include "BaseFramebuffer/Framebuffer.hpp"
#include "Queues/CommandPools.hpp"
#include "Textures/Texture.hpp"
#include "Materials/Manager.hpp"
#include "RenderSystems/Registry.hpp"
#include "WhiteTexture.hpp"
#include "Pipelines/LayoutRegistry.hpp"
#include "Pipelines/GraphicsPipelineRegistry.hpp"

namespace Raindrop::Graphics{
	struct Context{
		Context(::Raindrop::Context& core);
		~Context() = default;

		::Raindrop::Context& core;
		VkAllocationCallbacks* allocationCallbacks;

		Core::Window window;
		Core::Instance instance;
		Core::PhysicalDevice physicalDevice;
		Core::Device device;
		Queues::Manager queues;
		Queues::CommandPools commandPools;
		Core::Swapchain swapchain;
		Pipelines::LayoutRegistry pipelineLayoutRegistry;
		Pipelines::GraphicsPipelineRegistry graphicsPipelineRegistry;
		BaseFramebuffer::Framebuffer baseFramebuffer;
		WhiteTexture white;
		
		Materials::Manager materials;
		RenderSystems::Registry renderSystems;
	};
}

#endif