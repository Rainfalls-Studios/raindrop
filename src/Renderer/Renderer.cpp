#include <Raindrop/Renderer/Renderer.hpp>
#include <Raindrop/Renderer/Context.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Renderer{
	Renderer::Renderer(::Raindrop::Context& context){
		spdlog::info("Constructing renderer ...");
		_context = new Context(context);
	}

	Renderer::~Renderer(){
		spdlog::info("Destructing renderer ...");
		delete _context;
	}

	void Renderer::render(){

	}

	void Renderer::events(){
		_context->window.events();
	}
}