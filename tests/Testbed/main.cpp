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
		[](RD::Key scancode, RD::Key keycode, RD::KeyMod repeat) -> void {
			if (keycode == RD::Key::KEY_LCTRL){
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

		model.model = engine.getAsset<Raindrop::Graphics::Models::Model>("Model", fs::current_path() / "models/sponza/sponza.obj");
	}

	Raindrop::Camera camera;

	engine.subscribeEvent(
		"Renderer.swapchain.resized",
		[&camera](RD::Maths::uvec2 size) -> void {
			camera.setAspectRatio(static_cast<float>(size.x) / static_cast<float>(size.y));
			camera.updateProjection();
		}
	);

	engine.subscribeEvent(
		"mouse.motion",
		[&](RD::Maths::vec2 position, RD::Maths::vec2 relativePosition) -> void {
			if (SDL_GetRelativeMouseMode() == SDL_TRUE){
				auto& rotation = camera.rotation();

				const float sensitivity = 0.1f;

				float yaw = relativePosition.x * sensitivity;
				float pitch = -relativePosition.y * sensitivity;
				
				RD::Maths::quat yawRotation = RD::Maths::angleAxis(RD::Maths::radians(yaw), RD::Maths::normalize(RD::Maths::vec3(0.0f, 1.0f, 0.0f)));
				RD::Maths::quat pitchRotation = RD::Maths::angleAxis(RD::Maths::radians(pitch), RD::Maths::normalize(RD::Maths::vec3(1.0f, 0.0f, 0.0f)));

				RD::Maths::quat cameraRotation = camera.rotation();

				cameraRotation = yawRotation * cameraRotation;
				cameraRotation = cameraRotation * pitchRotation;

				cameraRotation = RD::Maths::normalize(cameraRotation);
				camera.setRotation(cameraRotation);
			}
		}
	);

	engine.subscribeEvent(
		"OnTick",
			[&]() -> void {
			auto& keys = engine.eventManager().keyEvents();
			auto& mouse = engine.eventManager().mouseEvents();

			using namespace RD::Core::Event;

			auto& translation = camera.translation();
			auto& rotation = camera.rotation();

			RD::Maths::vec3 forward = RD::Maths::rotate(rotation, RD::Maths::vec3(0.f, 0.f, -1.f));
			RD::Maths::vec3 up = RD::Maths::rotate(rotation, RD::Maths::vec3(0.f, 1.f, 0.f));
			RD::Maths::vec3 left = RD::Maths::rotate(rotation, RD::Maths::vec3(1.f, 0.f, 0.f));

			float factor = 1.f;//0.0005f;

			if (keys[KEY_W]){
				camera.translate(forward * factor);
			}

			if (keys[KEY_S]){
				camera.translate(-forward * factor);
			}

			if (keys[KEY_A]){
				camera.translate(left * factor);
			}

			if (keys[KEY_D]){
				camera.translate(-left * factor);
			}

			if (keys[KEY_SPACE]){
				camera.translate(up * factor);
			}

			if (keys[KEY_LSHIFT]){
				camera.translate(-up * factor);
			}
		}
	);

	engine.subscribeEvent(
		"Renderer.frame",
		[&](VkCommandBuffer commandBuffer) -> void {
			auto properties = scene.getProperty<ForwardRenderSceneProperties>();
			auto& data = properties->data;
			
			data.ambientColor = RD::Maths::vec4(RD::Maths::vec3(0.1), 1.0);
			data.viewProjection = camera.viewProjection();
			data.cameraPosition = RD::Maths::vec4(camera.translation() / 2.f, 0.0);

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

void test_modules(){
	namespace RD = Raindrop;
	namespace fs = std::filesystem;

	using fs::current_path;

	current_path(PATH);
	RD::Raindrop engine;
	auto& renderer = engine.renderer();

	auto module = engine.loadModule(RD::Path(MODULE_PATH) / "barebone");

	engine.subscribeEvent(
		"quit",
		[&engine]() -> void {
			engine.quit();
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
	// test_modules();

	std::cout << std::endl;
	std::cout << "==========================================" << std::endl;
	std::cout << "=                 TESTBED                =" << std::endl;
	std::cout << "==========================================" << std::endl;

	return 0;
}