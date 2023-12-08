#include <Raindrop/Raindrop.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

#include "config.hpp"

#include <Raindrop/Graphics/Engine.hpp>

int main(int argc, char** argv){
	Raindrop::Raindrop engine;

	std::filesystem::path source = RESOURCE_DIR;

	engine.subscribeEvent(
		"Graphics.Internal.Window.Quit",
		[&](){
			engine.exit();
		}
	);

	// engine.registry().put_child("SourceDir", boost::property_tree::ptree(RESOURCE_DIR));

	// try loading a shader
	// engine.loadOrGet(source / "pipelines/pipeline.ymlgfxpipe");
	
	engine.graphics().loadFromFile(source / "renderer.yaml");
	engine.run();
	
	return EXIT_SUCCESS;
}