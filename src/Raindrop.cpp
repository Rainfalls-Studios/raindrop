#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/Manager.hpp>

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine");
		_context = new Context();
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;
		
		auto a = [](int a){return a;};

		_context->eventManager.subscribe(
			"window.quit",
			[&]() -> void {
				_context->running = false;
			}
		);

		_context->eventManager.subscribe(
			"window.resized",
			std::function<void(int, int)>([](int w, int h) -> void {
				spdlog::info("window : {}, height : {}", w, h);
			})
		);

		while (_context->running){
			render();
			events();
		}
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destructing Raindrop engine");
		delete _context;
	}

	void Raindrop::render(){
		_context->renderer.render();
	}

	void Raindrop::events(){
		_context->renderer.events();
	}
}