#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine");
		_context = new Context();
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;

		_context->eventManager.subscribe(
			"window.quit",
			[&](){
				_context->running = false;
			}
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