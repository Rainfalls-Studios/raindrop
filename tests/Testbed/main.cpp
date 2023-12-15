#include <Raindrop/Raindrop.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

#include "config.hpp"

#include <Raindrop/Graphics/Engine.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>

int main(int argc, char** argv){
	Raindrop::Raindrop engine;

	std::filesystem::path source = RESOURCE_DIR;

	engine.subscribeEvent(
		"Graphics.Internal.Window.Quit",
		[&](){
			engine.exit();
		}
	);

	engine.subscribeEvent(
		"Renderer.preSwapchainRenderPass",
		[&](){
			auto& context = engine.graphics().context();

			auto commandBuffer = 
			auto pipeline = context.shaders().graphicsPipelineManager().get("default graphics pipeline");

		}
	);

	engine.registry().set("resourcesDir", RESOURCE_DIR);
	
	engine.graphics().loadFromFile(source / "renderer.yaml");
	engine.run();
	
	return EXIT_SUCCESS;
}