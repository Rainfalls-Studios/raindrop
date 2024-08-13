#include <Raindrop/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop");
	}

	void Context::initialize(){
		createLogger();
		initializeRegistry();

		initializeAssets();
		initializeEvents();
		initializeScenes();
		initializeGraphics();
	}

	void Context::initializeRegistry(){
		
	}

	void Context::initializeAssets(){
		assets = std::make_unique<Assets::Context>();
		assets->initialize(*this);
	}

	void Context::initializeEvents(){
		events = std::make_unique<Events::Context>();
		events->initialize(*this);
	}

	void Context::initializeGraphics(){
		graphics = std::make_unique<Graphics::Context>();
		graphics->initialize(*this);
	}

	void Context::initializeScenes(){
		scenes = std::make_unique<Scenes::Context>();
		scenes->initialize(*this);
	}
}