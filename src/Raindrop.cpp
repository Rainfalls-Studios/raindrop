#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/Manager.hpp>

#include <iostream>

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine ...");
		_context = new Context();
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;

		_context->eventManager.subscribe(
			"quit",
			[&]() -> void {
				_context->running = false;
			}
		);
		
		while (_context->running){
			render();
			events();
			updateCameraPosition();
		}
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destructing Raindrop engine ...");
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

		float factor = 0.0005f;

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

		if (keys[KEY_LSHIFT]){
			camera.translate(up * factor);
		}

		if (keys[KEY_LCTRL]){
			camera.translate(-up * factor);
		}
	}
}