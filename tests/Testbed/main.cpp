#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/SimpleRenderer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

#include "config.h"

struct PushConstant{
	glm::mat4 modelTransform = glm::mat4(1.f);
	glm::mat4 viewProjection = glm::mat4(1.f);
};

std::string read_file(const std::filesystem::path& path){
	spdlog::info("Loading file \"{}\" ...", path.string());
	std::ifstream file(path, std::ios_base::binary | std::ios_base::in);
	if (!file.is_open()){
		spdlog::warn("Could not open file \"{}\"", path.string());
		throw std::runtime_error("Could not open file");
	}

	std::stringstream streamBuf;
	streamBuf << file.rdbuf();

	spdlog::info("File \"{}\" loaded !", path.string());

	return streamBuf.str();
}

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

			_layout.prepare(engine.getGraphicsContext())
				.addPushConstant()
					.set<PushConstant>()
					.setStage(VK_SHADER_STAGE_VERTEX_BIT);
				
			_layout.initialize();

			_fragment.prepare(engine.getGraphicsContext());
			_fragment.setCode(read_file(PATH / "shaders/triangle/triangle.frag.spv"))
				.initialize();
			
			_vertex.prepare(engine.getGraphicsContext());
			_vertex.setCode(read_file(PATH / "shaders/triangle/triangle.vert.spv"))
				.initialize();
			
			_pipeline.prepare(engine.getGraphicsContext());
			_pipeline.addStage()
				.setModule(_fragment.get())
				.setEntryPoint("main")
				.setStage(VK_SHADER_STAGE_FRAGMENT_BIT);
			
			_pipeline.addStage()
				.setModule(_vertex.get())
				.setEntryPoint("main")
				.setStage(VK_SHADER_STAGE_VERTEX_BIT);
			
			_pipeline.setLayout(_layout)
				.setRenderPass(_engine->getGraphicsContext().window.swapchain.getRenderPass())
				.setSubpass(0);
			
			_pipeline.getViewportState().addScissor();
			_pipeline.getViewportState().addViewport();
			
			_pipeline.getDynamicState()
				.addDynamicStates({VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_VIEWPORT});

			_pipeline.getColorBlendState()
				.addColorAttachment()
					.enableBlending();
			
			_pipeline.getVertexInputState()
				.setLayout(layout);
		
			_pipeline.getInputAssemplyState()
				.setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
				
			_pipeline.initialize();
		}

		void bind(Raindrop::Graphics::CommandBuffer* cmd){
			_pipeline.bind(*cmd);
		}

		const Raindrop::Graphics::PipelineLayout& getLayout() const noexcept{
			return _layout;
		}

	private:
		Raindrop::Engine* _engine;
		Raindrop::Graphics::PipelineLayout _layout;
		Raindrop::Graphics::ShaderModule _fragment;
		Raindrop::Graphics::ShaderModule _vertex;
		Raindrop::Graphics::GraphicsPipeline _pipeline;
};

class Testbed : public Raindrop::Engine, public Raindrop::Events::Listener{
	public:
		bool closeEvent(const Raindrop::Events::WindowCloseRequest& event){
			_run = false;
			return true;
		}

		Testbed() : Engine(), Listener(){
			Engine::initialize(INIT_EVERYTHING);
			_renderer.initialize(getGraphicsContext());

			Engine::subscribeToEvent<Raindrop::Events::WindowCloseRequest>(this, &Testbed::closeEvent);

			auto layout = Engine::createGraphicsVertexLayout();
			layout.addBinding("binding")
				.addAttribute<glm::vec3>("position", Raindrop::Graphics::VertexLayout::POSITION)
				.addAttribute<glm::vec3>(
					"color",
					Raindrop::Graphics::VertexLayout::NORMAL,
					Raindrop::Graphics::VertexLayout::LOCATION_AUTO,
					
					Raindrop::Graphics::Formats::ComponentSwizzle{
						Raindrop::Graphics::Formats::ComponentType::G,
						Raindrop::Graphics::Formats::ComponentType::R,
						Raindrop::Graphics::Formats::ComponentType::R,
						Raindrop::Graphics::Formats::ComponentType::A
					});

			auto config = Engine::createGraphicsModelLayoutConfig();
			config.addLayout(layout)
				.require(Raindrop::Graphics::ModelLayoutConfig::Usage::POSITION);
			
			_model = Engine::getGraphicsModelLoader().load(PATH / "models/bunny.obj", config);

			_pipeline.initialize(*this, layout);
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
	Testbed testbed;

	testbed.run();

	return EXIT_SUCCESS;
}