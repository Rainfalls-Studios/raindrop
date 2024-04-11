#include <iostream>
#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Graphics/Renderer.hpp>
#include <Raindrop/Components/Transformation.hpp>
#include <Raindrop/Components/Model.hpp>

#include "config.h"
#include <SDL3/SDL.h>

#include "ForwardRenderSystem.hpp"

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

	auto scene = engine.createScene();

	auto renderSystem = engine.renderer().createRenderSystem<ForwardRenderSystem>(engine);
	renderSystem->bind(scene);
	
	{
		auto entity = scene.create();
		scene.emplaceComponent<Raindrop::Components::Transformation>(entity);
		auto& model = scene.emplaceComponent<Raindrop::Components::Model>(entity);

		model.model = engine.getAsset<Raindrop::Graphics::Models::Model>("Model", fs::current_path() / "models/Sponza/Sponza.gltf");
	}

	engine.subscribeEvent(
		"Renderer.frame",
		[&](VkCommandBuffer commandBuffer) -> void {
			auto properties = scene.getProperty<ForwardRenderSceneProperties>();
			
			properties->data.viewProjection = engine.camera().viewTransform();

			renderSystem->updateScene(commandBuffer, scene);
		}
	);

	engine.subscribeEvent(
		"Renderer.baseFramebuffer.renderPass",
		[&](VkCommandBuffer commandBuffer) -> void {
			renderSystem->render(commandBuffer, scene);
		}
	);

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