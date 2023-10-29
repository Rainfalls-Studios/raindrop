#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Core{
	Context::Context() : 
		_logger("Core"),
		_registry(*this),
		_temp(*this),
		_eventManager(*this),
		_assetManager(*this),
		_sceneManager(*this)
	{}
	
	spdlog::logger& Context::logger(){
		return _logger;
	}
	
	Registry::Registry& Context::registry(){
		return _registry;
	}

	Registry::Registry& Context::temp(){
		return _temp;
	}

	Event::EventManager& Context::eventManager(){
		return _eventManager;
	}

	Asset::AssetManager& Context::assetManager(){
		return _assetManager;
	}

	Scene::SceneManager& Context::sceneManager(){
		return _sceneManager;
	}
}