#include <Raindrop/Core/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Core{
	Context::Context() : 
		_logger(spdlog::stdout_logger_mt("Raindrop::Core"))
	{
		
		_logger->info("Loading Raindrop Core context...");

		_registry = std::make_unique<Registry::Registry>();
		_temp = std::make_unique<Registry::Registry>();
		_eventManager = std::make_unique<Event::EventManager>(*this);
		_assetManager = std::make_unique<Asset::AssetManager>(*this);
		_sceneManager = std::make_unique<Scene::SceneManager>(*this);

		_logger->info("Raindrop Core context loaded without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Raindrop Core context...");

		_eventManager.reset();
		_assetManager.reset();
		_sceneManager.reset();
		_temp.reset();
		_registry.reset();

		_logger->info("Raindrop Core context terminated without any critical error");
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}
	
	Registry::Registry& Context::registry(){
		return *_registry;
	}

	Registry::Registry& Context::temp(){
		return *_temp;
	}

	Event::EventManager& Context::eventManager(){
		return *_eventManager;
	}

	Asset::AssetManager& Context::assetManager(){
		return *_assetManager;
	}

	Scene::SceneManager& Context::sceneManager(){
		return *_sceneManager;
	}
}