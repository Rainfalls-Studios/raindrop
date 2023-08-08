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

#include <Raindrop/Graphics/ShadowMap/Sun/RenderPass.hpp>
#include <Raindrop/Graphics/ForwardShader/RenderPass.hpp>

namespace Raindrop::Graphics{

	struct DescriptorLayouts{
		VkDescriptorSetLayout sunShadowMapLayout;
		VkDescriptorSetLayout lightPointShadowMapLayout;
		VkDescriptorSetLayout spotlightShadowMapLayout;

		DescriptorLayouts(GraphicsContext& context);
		~DescriptorLayouts();

		private:
			GraphicsContext& _context;

			void createLightsLayouts();
			void destroyLightsLayouts();
	};

	struct RenderPasses{
		RenderPasses(GraphicsContext& context);
		~RenderPasses();

		ShadowMap::Sun::RenderPass sun;
		ForwardShader::RenderPass forwardShader;
	};

	struct GraphicsContext{

		GraphicsContext(Core::EngineContext& context, Core::Scene::Scene& scene);
		~GraphicsContext();

		GraphicsContext(const GraphicsContext &) = delete;
		GraphicsContext& operator=(const GraphicsContext &) = delete;

		Core::EngineContext* operator->() const{
			return &context;
		}
		
		Core::EngineContext& context;
		Core::Scene::Scene& scene;

		GraphicsRegistry gRegistry;
		VkAllocationCallbacks* allocationCallbacks = nullptr;

		Window window;
		Instance instance;
		Device device;
		Swapchain swapchain;

		GraphicsCommandPool graphicsCommandPool;
		TransfertCommandPool transfertCommandPool;

		VkDescriptorPool pool;

		VkRenderPass sceneRenderPass;

		uint32_t graphicsFamily;
		uint32_t transfertFamily;
		uint32_t presentFamily;

		VkQueue graphicsQueue;
		VkQueue transfertQueue;
		VkQueue presentQueue;

		DescriptorLayouts layouts;
		RenderPasses renderPasses;

		const Texture& whiteTexture() const; 

		private:
			void createPool();
			void destroyPool();
			void createWhiteTexture();

			std::unique_ptr<Texture> _whiteTexture;
	};
}

#endif