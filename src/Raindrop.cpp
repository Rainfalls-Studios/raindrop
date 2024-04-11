#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/Manager.hpp>

#include <iostream>
#include <SDL3/SDL.h>

glm::quat RotationQuaternion(float angle, const glm::vec3& axis) {
    return glm::angleAxis(glm::radians(angle), glm::normalize(axis));
}

// Function to rotate a vector by a quaternion
glm::vec3 RotateVector(const glm::vec3& vec, const glm::quat& rotation) {
    glm::quat conjugate = glm::conjugate(rotation);
    glm::quat rotated = rotation * glm::quat(0, vec) * conjugate;
    return glm::vec3(rotated.x, rotated.y, rotated.z);
}

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine ...");
		_context = new Context();
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;

		_context->eventManager.subscribe(
			"mouse.motion",
			[&](glm::vec2 position, glm::vec2 relativePosition) -> void {
				if (SDL_GetRelativeMouseMode() == SDL_TRUE){
					auto& camera = _context->camera;
					auto& rotation = camera.rotation();

					const float sensitivity = 0.1f;

					float yaw = relativePosition.x * sensitivity;
					float pitch = -relativePosition.y * sensitivity;
					
					glm::quat yawRotation = RotationQuaternion(yaw, glm::vec3(0.0f, 1.0f, 0.0f));
   					glm::quat pitchRotation = RotationQuaternion(pitch, glm::vec3(1.0f, 0.0f, 0.0f));

					glm::quat cameraRotation = camera.rotation();

					cameraRotation = yawRotation * cameraRotation;
					cameraRotation = cameraRotation * pitchRotation;

    				cameraRotation = glm::normalize(cameraRotation);
					camera.setRotation(cameraRotation);
				}
			}
		);
		
		while (_context->running){
			render();
			events();
			updateCameraPosition();
		}
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destroying Raindrop engine ...");
		delete _context;
	}

	void Raindrop::render(){
		_context->renderer.render();
	}

	void Raindrop::events(){
		_context->renderer.events();
	}

	void Raindrop::updateCameraPosition(){
		auto& keys = _context->eventManager.keyEvents();
		auto& mouse = _context->eventManager.mouseEvents();
		auto& camera = _context->camera;

		using namespace Core::Event;

		auto& translation = camera.translation();
		auto& rotation = camera.rotation();

		glm::vec3 forward = glm::rotate(rotation, glm::vec3(0.f, 0.f, -1.f));
		glm::vec3 up = glm::rotate(rotation, glm::vec3(0.f, 1.f, 0.f));
		glm::vec3 left = glm::rotate(rotation, glm::vec3(1.f, 0.f, 0.f));

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

	SceneWrapper Raindrop::createScene(){
		return SceneWrapper(*_context, _context->scenes.createScene());
	}

	SceneWrapper Raindrop::registerScene(std::unique_ptr<Scene> scene){
		return SceneWrapper(*_context, _context->scenes.registerScene(std::move(scene)));
	}

	void Raindrop::unregisterScene(const SceneWrapper& wrapper){
		return _context->scenes.unregisterScene(wrapper.ID());
	}

	SceneWrapper Raindrop::getScene(const SceneID& ID){
		return SceneWrapper(*_context, ID);
	}

	AssetManager& Raindrop::assetManager(){
		return _context->assetManager;
	}

	EventManager& Raindrop::eventManager(){
		return _context->eventManager;
	}

	Graphics::Renderer& Raindrop::renderer(){
		return _context->renderer;
	}

	void Raindrop::quit(){
		_context->running = false;
	}

	std::shared_ptr<Asset> Raindrop::getAsset(const std::string& type, const Path& path){
		return _context->assetManager.get(type, path);
	}

	void Raindrop::registerAssetLoader(const std::string& type, const std::shared_ptr<AssetLoader>& loader){
		return _context->assetManager.registerLoader(type, loader);
	}

	void Raindrop::unregisterAssetLoader(const std::string& type){
		_context->assetManager.unregisterType(type);
	}

	std::shared_ptr<AssetLoader> Raindrop::getAssetLoader(const std::string& type){
		return _context->assetManager.findLoader(type);
	}

	Camera& Raindrop::camera(){
		return _context->camera;
	}

}