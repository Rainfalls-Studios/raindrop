#include <Raindrop/Raindrop.hpp>

INITIALIZE_EASYLOGGINGPP;

namespace Raindrop{
	Raindrop::Raindrop() : 
		_renderer{_context}{
	}

	Raindrop::~Raindrop(){

	}

	void Raindrop::run(){
		_launched = true;
		while (_launched){
			render();
		}
	}
	
	Scene& Raindrop::scene(){
		return _scene;
	}

	const Scene& Raindrop::scene() const{
		return _scene;
	}

	void Raindrop::setScene(const Scene& scene){
		_scene = scene;
	}

	Scene Raindrop::createScene(){
		return Scene(std::make_shared<Core::Scene::Scene>(_context, 5000, 100));
	}

	void Raindrop::render(){
		Graphics::FrameState frameState = _renderer.begin();
		if (!frameState) return;

		_renderer.renderScene(frameState, _scene);

		_renderer.end(frameState);
	}
}