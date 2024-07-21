#include <Raindrop/Scenes/Scene.hpp>
#include <Raindrop/Scenes/Context.hpp>
#include <Raindrop/Scenes/Property.hpp>

namespace Raindrop::Scenes{

	Scene::Scene() noexcept : 
		_context{nullptr},
		_registry{},
		_properties{}
	{}

	Scene::~Scene(){
		release();
	}

	void Scene::prepare(Context& context){
		_context = &context;
	}

	void Scene::initialize(){

	}

	void Scene::release(){
		_properties.clear();
		_registry.clear();
		_context = nullptr;
	}

	entt::registry& Scene::getRegistry() noexcept{
		return _registry;
	}
	
	const entt::registry& Scene::getRegistry() const noexcept{
		return _registry;
	}
}