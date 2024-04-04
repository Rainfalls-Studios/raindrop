#include <iostream>
#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Components/Transformation.hpp>
#include <Raindrop/Components/Model.hpp>

#include "config.h"
#include <SDL3/SDL.h>

void testbed(){
	namespace RD = Raindrop;
	namespace fs = std::filesystem;

	using fs::current_path;

	current_path(PATH);
	RD::Raindrop engine;
	auto& renderer = engine.renderer();

	engine.subscribeEvent(
		"quit",
		[&engine]() -> void {
			engine.quit();
		}
	);

	engine.subscribeEvent(
		"key.down",
		[](SDL_Scancode scancode, SDL_Keycode keycode, uint16_t repeat) -> void {
			if (keycode == SDLK_LCTRL){
				if (SDL_GetRelativeMouseMode() == SDL_TRUE){
					SDL_SetRelativeMouseMode(SDL_FALSE);
				} else {
					SDL_SetRelativeMouseMode(SDL_TRUE);
				}
			}
		}
	);

	// auto entity = RD::Entity::create(engine.scene());

	// entity.emplace<RD::Components::Model>().model = engine.assetManager().get<RD::Renderer::Model::Model>("Model", current_path() / "models/Sponza/Sponza.gltf");
	// entity.emplace<RD::Components::Transformation>();

	engine.run();
}

int main(int argc, char** argv){
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << std::endl;
	
	testbed();

	std::cout << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;

	return 0;
}