#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Core/Engine.hpp>
#include <Raindrop/Graphics/Engine.hpp>

namespace Raindrop{
	Raindrop::Raindrop(){
		_core = std::make_unique<Core::Engine>();
		_graphics = std::make_unique<Graphics::Engine>(*_core);
	}

	Raindrop::~Raindrop(){
		_graphics.reset();
		_core.reset();
	}

	void Raindrop::run(){
		_launched = true;
		while (_launched){
			triggerEvent("OnTick");

			_graphics->render();
		}
	}

	Scene Raindrop::createScene(const std::string& name){
		return Scene(_core->createScene(name));
	}

	void Raindrop::triggerEvent(const std::string& event){
		_core->triggerEvent(event);
	}

	void Raindrop::subscribeEvent(const std::string& event, const EventCallback& callback){
		_core->subscribeEvent(event, callback);
	}	

	Scene Raindrop::createOrGetScene(const std::string& name){
		return Scene(_core->getOrCreateScene(name));
	}

	Scene Raindrop::getScene(const std::string& name){
		return Scene(_core->getScene(name));
	}

	void Raindrop::removeScene(const std::string& name){
		_core->removeScene(name);
	}
	
	void Raindrop::removeScene(const Scene& scene){
		_core->removeScene(scene._scene);
	}

	Asset Raindrop::loadOrGet(const std::string& path){
		return _core->loadOrGet(path);
	}

	void Raindrop::exit(){
		_launched = false;
	}

	
	// Scene& Raindrop::scene(){
	// 	return _scene;
	// }

	// const Scene& Raindrop::scene() const{
	// 	return _scene;
	// }

	// void Raindrop::setScene(const Scene& scene){
	// 	_scene = scene;
	// }

	// Scene Raindrop::createScene(){
	// 	std::shared_ptr<Core::Scene::Scene> scene = std::make_shared<Core::Scene::Scene>(_context, 5000, 100);
	// 	_renderer.registerSceneComponents(*scene);
	// 	return Scene(scene);
	// }

	// void Raindrop::render(){
	// 	Graphics::FrameState frameState = _renderer.begin();
	// 	if (!frameState) return;

	// 	_renderer.renderScene(frameState, _scene);

	// 	_renderer.end(frameState);
	// }
}