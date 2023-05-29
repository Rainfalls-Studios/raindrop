#include <Raindrop/Core/Engine.hpp>

INITIALIZE_EASYLOGGINGPP

namespace Raindrop::Core{
	Engine::Engine(){
		el::Logger* customLogger = el::Loggers::getLogger("Engine");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine") << "Creating New Engine ...";

		_eventManager = std::make_unique<Core::Event::EventManager>();
		_assetManager = std::make_unique<Core::Asset::AssetManager>();
		_registryManager = std::make_unique<Core::Registry::RegistryManager>();
		
		_scene = std::make_unique<Core::Scene::Scene>(registryManager());

		_renderer = std::make_unique<Graphics::Renderer>(eventManager(), assetManager());

		setupEvents();
		_eventManager->subscribe("engine.window.quit", [&](){
			launched = false;
		});

		CLOG(INFO, "Engine") << "Engine created with success !";
	}

	Engine::~Engine(){
		CLOG(INFO, "Engine") << "Destroying Engine ...";
	}

	Event::EventManager& Engine::eventManager(){
		return *_eventManager.get();
	}

	Registry::RegistryManager& Engine::registryManager(){
		return *_registryManager.get();
	}

	Scene::Scene& Engine::scene(){
		return *_scene.get();
	}

	Asset::AssetManager& Engine::assetManager(){
		return *_assetManager.get();
	}
	
	void Engine::mainloop(){
		CLOG(INFO, "Engine") << "Starting engine mainloop";
		launched = true;

		_eventManager->trigger("engine.loop.start");
		while (launched){
			_eventManager->trigger("engine.loop.tick");
			_renderer->update();
		}
		_eventManager->trigger("engine.loop.end");
	}

	void Engine::setupEvents(){
		_eventManager->subscribe("engine.quit", [&](){
			launched = false;
		});
	}
}