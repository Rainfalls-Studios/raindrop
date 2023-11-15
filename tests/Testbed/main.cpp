#include <Raindrop/Raindrop.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

#include "config.hpp"

int main(int argc, char** argv){
	Raindrop::Raindrop engine;

	std::filesystem::path source = RESOURCE_DIR;

	engine.subscribeEvent(
		"Graphics.Internal.Window.Quit",
		[&](){
			engine.exit();
		}
	);

	// try loading a shader
	engine.loadOrGet(source / "shaders/shader.vert.glsl.spv");
	engine.run();
	
	return EXIT_SUCCESS;
}