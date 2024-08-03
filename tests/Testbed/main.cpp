#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/SimpleRenderer.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>

#include "config.h"

class CustomSceneProperty : public Raindrop::Scenes::Property{
	public:
		int myValue;
};

struct PushConstant{
	int a;
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

		void initialize(Raindrop::Engine& engine){
			_engine = &engine;

			_layout.prepare(engine.getGraphicsContext());
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
				
			_pipeline.initialize();
		}

		void bind(Raindrop::Graphics::CommandBuffer* cmd){
			_pipeline.bind(*cmd);
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

			Raindrop::Graphics::VertexLayout layout;
			{
				auto binding = layout.addBinding("binding 0");

				layout.addAttribute<glm::vec3>(binding, "color")
					.addAttribute<glm::vec3>(binding, "position")
					.addAttribute<glm::vec2>(binding, "uv");

				std::cout << layout["binding 0"]["color"].name << std::endl;
			}


			auto meshData = Engine::createGraphicsMeshData();
			{
				meshData.setLayout(layout);
				meshData.allocate(10);

				auto binding = meshData.getBinding("binding 0");

				auto color = binding.get<glm::vec3>("color");
				color[0] = glm::vec3(0.f);
			}

			_pipeline.initialize(*this);
		}

		void run(){
			_run = true;
			auto start = std::chrono::steady_clock::now();

			while (_run){
				auto now = std::chrono::steady_clock::now();

				std::chrono::duration<float> duration = now - start;
				Engine::getGraphicsWindow().events();

				auto commandBuffer = _renderer.beginFrame();
				if (commandBuffer){

					glm::vec3 clearColor;
					clearColor.r = glm::sin(duration.count());
					clearColor.g = glm::cos(duration.count() * 3.14);
					clearColor.b = glm::sin(duration.count() * 1.5);

					_renderer.setSwapchainColor(clearColor);
					_renderer.beginSwapchainRenderPass(commandBuffer);

					_pipeline.bind(commandBuffer);
					vkCmdDraw(commandBuffer->get(), 3, 1, 0, 0);

					_renderer.endSwapchainRenderPass(commandBuffer);

					_renderer.endFrame(commandBuffer);
				}
			}
		}

	private:
		Raindrop::Graphics::SimpleRenderer _renderer;
		TrianglePipeline _pipeline;
		bool _run;
};

int main(){
	Testbed testbed;

	testbed.run();

	return EXIT_SUCCESS;
}