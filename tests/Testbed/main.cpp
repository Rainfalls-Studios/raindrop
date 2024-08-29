#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/SimpleRenderer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/Graphics/PipelineLayout.hpp>
#include <Raindrop/Graphics/ShaderModule.hpp>
#include <Raindrop/Graphics/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/VertexLayout.hpp>
#include <Raindrop/Graphics/ModelLayoutConfig.hpp>
#include <Raindrop/Graphics/Model.hpp>
#include <Raindrop/Graphics/Mesh.hpp>
#include <Raindrop/Graphics/Buffer.hpp>
#include <Raindrop/Graphics/Texture.hpp>
#include <Raindrop/Events/Listener.hpp>

#include <spdlog/spdlog.h>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#include "config.h"

struct PushConstant{
	glm::mat4 modelTransform = glm::mat4(1.f);
	glm::mat4 viewProjection = glm::mat4(1.f);
};

class TrianglePipeline{
	public:
		TrianglePipeline() noexcept : 
			_engine{nullptr},
			_layout{},
			_fragment{},
			_vertex{},
			_pipeline{}
		{}

		~TrianglePipeline(){
			_engine->getGraphicsContext().getDevice().waitIdle();
		}

		void initialize(Raindrop::Engine& engine, const Raindrop::Graphics::VertexLayout& layout){
			_engine = &engine;

			_layout = engine.createGraphicsPipelineLayout();
			_layout->addPushConstant()
					.set<PushConstant>()
					.setStage(VK_SHADER_STAGE_VERTEX_BIT);
				
			_layout->initialize();

			_fragment = engine.loadOrGet<Raindrop::Graphics::ShaderModule>(PATH / "shaders/triangle/triangle.frag.spv");
			_vertex = engine.loadOrGet<Raindrop::Graphics::ShaderModule>(PATH / "shaders/triangle/triangle.vert.spv");
			
			_pipeline = engine.createGraphicsGraphicsPipeline();
			_pipeline->addStage()
				.setModule(_fragment)
				.setEntryPoint("main")
				.setStage(VK_SHADER_STAGE_FRAGMENT_BIT);
			
			_pipeline->addStage()
				.setModule(_vertex)
				.setEntryPoint("main")
				.setStage(VK_SHADER_STAGE_VERTEX_BIT);
			
			_pipeline->setLayout(_layout)
				.setRenderPass(_engine->getGraphicsContext().window.swapchain.getRenderPass())
				.setSubpass(0);
			
			_pipeline->getViewportState().addScissor();
			_pipeline->getViewportState().addViewport();
			
			_pipeline->getDynamicState()
				.addDynamicStates({VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT});

			_pipeline->getColorBlendState()
				.addColorAttachment()
					.enableBlending();
			
			_pipeline->getVertexInputState()
				.setLayout(layout);
		
			_pipeline->getInputAssemplyState()
				.setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
				
			_pipeline->initialize();
		}

		void bind(Raindrop::Graphics::CommandBuffer* cmd){
			_pipeline->bind(*cmd);
		}

		const Raindrop::Graphics::PipelineLayout& getLayout() const noexcept{
			return *_layout;
		}

	private:
		Raindrop::Engine* _engine;
		std::shared_ptr<Raindrop::Graphics::PipelineLayout> _layout;
		std::shared_ptr<Raindrop::Graphics::ShaderModule> _fragment;
		std::shared_ptr<Raindrop::Graphics::ShaderModule> _vertex;
		std::shared_ptr<Raindrop::Graphics::GraphicsPipeline> _pipeline;
};

class Testbed : public Raindrop::Engine, public Raindrop::Events::Listener{
	public:
		bool closeEvent(const Raindrop::Events::WindowCloseRequest& event){
			_run = false;
			return true;
		}

		Testbed() : Engine(), Listener(){
			Engine::initialize();
			_renderer.initialize(getGraphicsContext());

			Engine::subscribeToEvent<Raindrop::Events::WindowCloseRequest>(this, &Testbed::closeEvent);

			auto layout = Engine::createGraphicsVertexLayout();
			layout.addBinding("binding")
				.addAttribute<glm::vec3>("position", Raindrop::Graphics::VertexLayout::POSITION)
				.addAttribute<glm::vec3>("color", Raindrop::Graphics::VertexLayout::NORMAL);
				
			auto config = Engine::createGraphicsModelLayoutConfig();
			config.addLayout(layout)
				.require(Raindrop::Graphics::ModelLayoutConfig::Usage::POSITION);
			
			_model = Engine::getGraphicsModelLoader().load(PATH / "models/bunny.obj", config);

			_pipeline.initialize(*this, layout);

			auto texture = Engine::createGraphicsTexture();
			texture->setWidth(1024)
				.setHeight(1024)
				.setFormat(VK_FORMAT_R8G8B8A8_SINT)
				.setLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
				.allocate();
		}

		~Testbed(){
			Engine::getGraphicsDevice().waitIdle();
		}

		void run(){
			_run = true;
			auto start = std::chrono::steady_clock::now();
			PushConstant p;

			_renderer.setSwapchainColor(glm::vec3(0.2f));
			while (_run){
				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> duration = now - start;
				Engine::getGraphicsWindow().events();

				auto commandBuffer = _renderer.beginFrame();
				if (commandBuffer){

					p.modelTransform = glm::scale(glm::mat4(1.f), glm::vec3(std::sin(duration.count()) * 0.3 + 0.3));

					_renderer.beginSwapchainRenderPass(commandBuffer);

					_pipeline.bind(commandBuffer);
					for (auto& mesh : _model->getMeshes()){
						if (mesh.hasIndexBuffer()){
							vkCmdBindIndexBuffer(commandBuffer->get(), mesh.getIndexBuffer()->get(), 0, mesh.getIndexType());
						}

						const auto& buffers = mesh.getVertexBuffers();
						std::vector<VkBuffer> vkBuffers(buffers.size());
						std::vector<VkDeviceSize> offsets(buffers.size());

						for (std::size_t i=0; i<buffers.size(); i++){
							vkBuffers[i] = buffers[i].get();
							offsets[i] = 0;
						}

						vkCmdBindVertexBuffers(commandBuffer->get(), 0, vkBuffers.size(), vkBuffers.data(), offsets.data());

						vkCmdPushConstants(commandBuffer->get(), _pipeline.getLayout().get(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &p);
						vkCmdDraw(commandBuffer->get(), mesh.getVertexCount(), 1, 0, 0);
					}

					_renderer.endSwapchainRenderPass(commandBuffer);

					_renderer.endFrame(commandBuffer);
				}
			}
		}

	private:
		Raindrop::Graphics::SimpleRenderer _renderer;
		TrianglePipeline _pipeline;
		bool _run;

		std::shared_ptr<Raindrop::Graphics::Model> _model;
};

int main(){
	try{
		Testbed testbed;
		testbed.run();
	} catch (const std::exception& e){
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}