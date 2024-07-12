#include <Raindrop/Engine.hpp>

namespace Raindrop{
	Engine::Engine() :
		_context{nullptr},
		_graphics{nullptr}
	{}

	Engine::~Engine(){
		release();
	}


	void Engine::initialize(InitFlags flags){
		initializeCore();
		

		if (flags & GRAPHICS){
			initializeGraphics();
		}
	}

	void Engine::release(){
		_context.reset();
	}



	void Engine::initializeCore(){
		_context = std::make_unique<Context>();

	}

	void Engine::initializeGraphics(){
		_graphics = std::make_unique<Graphics::Context>();
		_graphics->initialize();

		// _graphics->core.createLogger();
		// _graphics->core.instance.initialize(_graphics->core);
	}

}