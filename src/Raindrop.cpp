#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Context.hpp>
#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/Manager.hpp>

#include <iostream>
#include <SDL3/SDL.h>

namespace Raindrop{
	Raindrop::Raindrop(){
		spdlog::info("Constructing Raindrop engine ...");
		_context = new Context();

		registerEvent("OnTick");
	}

	void Raindrop::run(){
		spdlog::info("Starting to run...");
		_context->running = true;
		
		while (_context->running){
			triggerEvent("OnTick");
			render();
			events();
			updateCameraPosition();
		}
	}

	Raindrop::~Raindrop(){
		spdlog::info("Destroying Raindrop engine ...");
		delete _context;
	}

	void Raindrop::render(){
		_context->renderer.render();
	}

	void Raindrop::events(){
		_context->renderer.events();
	}

	void Raindrop::updateCameraPosition(){
		
	}

	SceneWrapper Raindrop::createScene(){
		return SceneWrapper(*_context, _context->scenes.createScene());
	}

	SceneWrapper Raindrop::registerScene(std::unique_ptr<Scene> scene){
		return SceneWrapper(*_context, _context->scenes.registerScene(std::move(scene)));
	}

	void Raindrop::unregisterScene(const SceneWrapper& wrapper){
		return _context->scenes.unregisterScene(wrapper.ID());
	}

	SceneWrapper Raindrop::getScene(const SceneID& ID){
		return SceneWrapper(*_context, ID);
	}

	AssetManager& Raindrop::assetManager(){
		return _context->assetManager;
	}

	EventManager& Raindrop::eventManager(){
		return _context->eventManager;
	}

	Graphics::Renderer& Raindrop::renderer(){
		return _context->renderer;
	}

	void Raindrop::quit(){
		_context->running = false;
	}

	std::shared_ptr<Asset> Raindrop::getAsset(const std::string& type, const Path& path){
		return _context->assetManager.get(type, path);
	}

	void Raindrop::registerAssetLoader(const std::string& type, const std::shared_ptr<AssetLoader>& loader){
		return _context->assetManager.registerLoader(type, loader);
	}

	void Raindrop::unregisterAssetLoader(const std::string& type){
		_context->assetManager.unregisterType(type);
	}

	std::shared_ptr<AssetLoader> Raindrop::getAssetLoader(const std::string& type){
		return _context->assetManager.findLoader(type);
	}
}