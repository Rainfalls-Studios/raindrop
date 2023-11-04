#ifndef __RAINDROP_GRAPHICS_ENGINE_HPP__
#define __RAINDROP_GRAPHICS_ENGINE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Engine{
		public:
			Engine(Core::Engine& core);
			~Engine();


		private:
			std::unique_ptr<Context> _context;

			// std::unique_ptr<BaseRender::WireMesh::Renderer> _wireMesh;
			// Factory::Factories _factories;

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