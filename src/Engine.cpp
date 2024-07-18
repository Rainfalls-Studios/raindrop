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

		if (flags & INIT_EVENTS){
			initializeEvents();
		}

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
		_graphics->initialize(getEventsContext());

		// _graphics->core.createLogger();
		// _graphics->core.instance.initialize(_graphics->core);
	}

	void Engine::initializeEvents(){
		_events = std::make_unique<Events::Context>();
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

	Events::Context& Engine::getEventsContext(){
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}

	const Events::Context& Engine::getEventsContext() const{
		if (!_events){
			throw std::runtime_error("Events context has not been initialized");
		}
		return *_events;
	}

}