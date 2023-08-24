#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/FrameState.hpp>
#include <Raindrop/Graphics/SceneRenderer.hpp>

#include <Raindrop/Core/Scene/Scene.hpp>

#ifdef RAINDROP_EDITOR
	#include <Raindrop/Graphics/Editor/common.hpp>
#endif

namespace Raindrop::Graphics{
	class Renderer{
		public:
			Renderer(Core::EngineContext& context);
			~Renderer();

			void registerSceneComponents(Core::Scene::Scene& scene);

			FrameState begin();
			void end(FrameState& state);

			void renderScene(FrameState& state, const Scene& scene);

		private:
			GraphicsContext _context;
			// SceneRenderer _sceneRenderer;

			uint32_t currentFrameID();

			void renderSceneWithCamera(FrameState& state, const Scene& scene, Camera& camera);



			// std::vector<VkCommandBuffer> _graphicsCommandBuffers;

			// std::shared_ptr<Factory::GraphicsPipelineFactory> _graphicsPipelineFactory;
			// std::shared_ptr<Factory::ShaderFactory> _shaderFactory; 
			// std::shared_ptr<Factory::ModelFactory> _modelFactory;
			// std::shared_ptr<Factory::TextureFactory> _textureFactory;

			// std::unique_ptr<WorldFramebuffer> _worldFramebuffer;
			// std::unique_ptr<SceneRenderer> _sceneRenderer;
			// std::unique_ptr<ForwardShader::ForwardShader> _forwardShader;

			// std::shared_ptr<GraphicsPipeline> _pipeline;
			// std::shared_ptr<DescriptorPool> _descriptorPool;
			// std::shared_ptr<DescriptorSetLayout> _setLayout;
			// VkDescriptorSet _descriptorSet;

			// std::unique_ptr<Pipelines> _pipelines;

			// void registerFactories();
			// void registerShaderFactory();
			// void registerGraphicsPipelineFactory();
			// void registerModelFactory();
			// void registerTextureFactory();
			// void eraseFactories();

			// void createGraphicsCommandBuffers();
			// void destroyGraphicsCommandBuffers();
			
			// VkCommandBuffer getCurrentGraphicsCommandBuffer();
			
			// VkCommandBuffer beginFrame();
			// void endFrame();

			// void renderGui();
			// void renderScene(VkCommandBuffer commmandBuffer);
			// void renderSwapchain(VkCommandBuffer commandBuffer);
			// void renderFrame(VkCommandBuffer commandBuffer);
			// void renderDepthMaps(VkCommandBuffer commandBuffer);
			// void renderSunDepthMap(VkCommandBuffer commandBuffer, Core::Scene::Entity sun);

			// void createDescriptorPool();
			// void createSetLayout();
			// void createPipeline();
			// void createDescriptorSet();

			// #ifdef RAINDROP_EDITOR
			// 	std::unique_ptr<Editor::Editor> _editor;
			// #endif
	};
}

#endif