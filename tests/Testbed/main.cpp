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

	engine.registry().set("resourcesDir", RESOURCE_DIR);

	auto& graphicsContext = engine.graphics().context();
	auto& registry = engine.registry();

	engine.subscribeEvent(
		"Renderer.SwapchainRenderPass",
		[&](){
			auto commandBuffer = engine.graphics().currentFramebuffer();
			auto pipeline = graphicsContext.shaders().graphicsPipelineManager().get("default graphics pipeline");
			if (!pipeline) return;

			pipeline->bind(commandBuffer);
			vkCmdDraw(commandBuffer, 6, 1, 0, 0);
		}
	);

	
	engine.graphics().loadFromFile(source / "renderer.yaml");
	engine.run();
	
	return EXIT_SUCCESS;
}