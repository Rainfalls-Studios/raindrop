#include <Raindrop/Raindrop.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop{
	Raindrop::Raindrop(){
		// _scene = createScene();

		// _context.eventManager.subscribe("Engine.Window.Quit", [this](){
		// 	_launched = false;
		// });
		_core = std::make_unique<Core::Engine>();
		_graphics = std::make_unique<Graphics::Engine>(*_core);
	}

	Raindrop::~Raindrop(){
		_core.reset();
	}

	// void Raindrop::run(){
	// 	_launched = true;
	// 	while (_launched){
	// 		render();
	// 	}
	// }
	
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