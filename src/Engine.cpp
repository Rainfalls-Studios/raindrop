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

		if (flags & INIT_GRAPHICS){
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



	// ======================= CORE ==============================

	Context& Engine::getContext(){
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	const Context& Engine::getContext() const{
		if (!_context){
			throw std::runtime_error("Core has not been initialized");
		}
		return *_context;
	}

	// ==================== GRAPHICS ============================

	Graphics::Context& Engine::getGraphicsContext(){
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}

	const Graphics::Context& Engine::getGraphicsContext() const{
		if (!_graphics){
			throw std::runtime_error("Graphics context has not been initialized");
		}
		return *_graphics;
	}


	Graphics::Core::Context& Engine::getGraphicsCoreContext(){
		return getGraphicsContext().core;
	}

	const Graphics::Core::Context& Engine::getGraphicsCoreContext() const{
		return getGraphicsContext().core;
	}


	Graphics::Window::Context& Engine::getGraphicsWindowContext(){
		return getGraphicsContext().window;
	}

	const Graphics::Window::Context& Engine::getGraphicsWindowContext() const{
		return getGraphicsContext().window;
	}
}