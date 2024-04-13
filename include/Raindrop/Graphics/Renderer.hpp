#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include "common.hpp"
#include "Materials/Manager.hpp"
#include "RenderSystems/RenderSystemRegistry.hpp"
#include "RenderSystems/RenderSystem.hpp"

#include "Wrappers/MaterialWrapper.hpp"
#include "Wrappers/RenderSystemWrapper.hpp"
#include "Wrappers/PipelineLayoutWrapper.hpp"
#include "Wrappers/GraphicsPipelineWrapper.hpp"

namespace Raindrop::Graphics{
	using namespace Wrappers;
	using Material = Materials::Material;
	using RenderSystem = RenderSystems::RenderSystem;
	using PipelineLayout = Pipelines::PipelineLayout;
	using GraphicsPipeline = Pipelines::GraphicsPipeline;

	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			// === Materials ===

			MaterialWrapper createMaterial();
			MaterialWrapper registerMaterial(const Material& material);
			void destroyMaterial(const MaterialWrapper& wrapper);

			Materials::Manager& materials();
			const Materials::Manager& materials() const;

			// === Render systems ===

			template<typename T, typename... Args>
			RenderSystemWrapper<T> createRenderSystem(Args&&... args){
				auto ID = renderSystems().create<T, Args...>(std::forward<Args>(args)...);
				return RenderSystemWrapper<T>(*_context, ID);
			}

			RenderSystemWrapper<> registerRenderSystem(std::unique_ptr<RenderSystem>&& system);
			void destroyRenderSystem(const RenderSystemWrapper<>& wrapper);

			RenderSystems::RenderSystemRegistry& renderSystems();
			const RenderSystems::RenderSystemRegistry& renderSystems() const;

			// === Pipeline layouts ===

			PipelineLayoutWrapper createPipelineLayout(const Pipelines::PipelineLayoutConfigInfo& info);
			void destroyPipelineLayout(const PipelineLayoutWrapper& wrapper);

			Pipelines::LayoutRegistry& pipelineLayouts();
			const Pipelines::LayoutRegistry& pipelineLayouts() const;

			// == Graphics pipelines ===

			GraphicsPipelineWrapper createGraphicsPipeline(const Pipelines::GraphicsPipelineConfigInfo& info);
			void destroyGraphicsPipeline(const GraphicsPipelineWrapper& wrapper);

			Pipelines::GraphicsPipelineRegistry& graphicsPipelines();
			const Pipelines::GraphicsPipelineRegistry& graphicsPipelines() const;

			// === Runtime ===

			void render();
			void events();

			Context& context();

		private:
			Context* _context;

			VkCommandBuffer beginFrame();
			void endFrame();

			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			void updateSwapchainSize();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;
	};
}

#endif