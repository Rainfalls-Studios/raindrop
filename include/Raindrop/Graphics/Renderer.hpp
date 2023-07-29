#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/factory/ShaderFactory.hpp>
#include <Raindrop/Graphics/factory/GraphicsPipelineFactory.hpp>
#include <Raindrop/Graphics/factory/ModelFactory.hpp>
#include <Raindrop/Graphics/factory/TextureFactory.hpp>
#include <Raindrop/Graphics/GUI/Interpreter.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

#ifdef RAINDROP_EDITOR
	#include <Raindrop/Graphics/Editor/common.hpp>
#endif

namespace Raindrop::Graphics{
	class Renderer{
		friend class ImGUI;
		public:
			Renderer(Core::EngineContext& context, Core::Scene::Scene& scene);
			~Renderer();

			void update();
			void openGUI(const std::filesystem::path& path);

		private:
			std::unique_ptr<GraphicsContext> _context;
			std::unique_ptr<ImGUI> _gui;
			GUI::Interpreter _interpreter;

			std::vector<VkCommandBuffer> _graphicsCommandBuffers;

			std::shared_ptr<Factory::GraphicsPipelineFactory> _graphicsPipelineFactory;
			std::shared_ptr<Factory::ShaderFactory> _shaderFactory; 
			std::shared_ptr<Factory::ModelFactory> _modelFactory;
			std::shared_ptr<Factory::TextureFactory> _textureFactory;

			std::unique_ptr<WorldFramebuffer> _worldFramebuffer;
			std::unique_ptr<SceneRenderer> _sceneRenderer;
			std::unique_ptr<ForwardShader> _forwardShader;

			std::shared_ptr<GraphicsPipeline> _pipeline;
			std::shared_ptr<DescriptorPool> _descriptorPool;
			std::shared_ptr<DescriptorSetLayout> _setLayout;
			VkDescriptorSet _descriptorSet;

			void registerFactories();
			void registerShaderFactory();
			void registerGraphicsPipelineFactory();
			void registerModelFactory();
			void registerTextureFactory();
			void eraseFactories();

			void createGraphicsCommandBuffers();
			void destroyGraphicsCommandBuffers();
			
			VkCommandBuffer getCurrentGraphicsCommandBuffer();
			
			VkCommandBuffer beginFrame();
			void endFrame();

			void renderGui();
			void renderScene(VkCommandBuffer commmandBuffer);
			void renderSwapchain(VkCommandBuffer commandBuffer);
			void renderFrame(VkCommandBuffer commandBuffer);

			void createDescriptorPool();
			void createSetLayout();
			void createPipeline();
			void createDescriptorSet();


			#ifdef RAINDROP_EDITOR
				std::unique_ptr<Editor::Editor> _editor;
			#endif
	};
}

#endif