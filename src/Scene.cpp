#include <Raindrop/Scene.hpp>
#include <Raindrop/Entity.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>

namespace Raindrop{
	Scene::Scene(const std::shared_ptr<Core::Scene::Scene>& scene) : _scene{scene}{}
	Scene::Scene(const Scene& other) : _scene{other._scene}{}
	Scene::Scene() : _scene{nullptr}{}
	Scene::~Scene(){}

	Scene& Scene::operator=(const std::shared_ptr<Core::Scene::Scene>& scene){
		_scene = scene;
		return *this;
	}

	Scene& Scene::operator=(const Scene& other){
		_scene = other._scene;
		return *this;
	}

	Entity Scene::root(){
		if (_scene)
			return Entity(Core::Scene::Entity(_scene->root(), _scene.get()));
		return Entity();
	}

	const Entity Scene::root() const{
		if (_scene)
			return Entity(Core::Scene::Entity(_scene->root(), _scene.get()));
		return Entity();
	}

	bool Scene::operator==(const std::shared_ptr<Core::Scene::Scene>& scene) const{
		return _scene == scene;
	}

	bool Scene::operator==(const Scene& other) const{
		return _scene == other._scene;
	}

	bool Scene::operator!=(const std::shared_ptr<Core::Scene::Scene>& scene) const{
		return _scene != scene;
	}

	bool Scene::operator!=(const Scene& other) const{
		return _scene != other._scene;
	}

	Entity Scene::createEntity(){
		return createEntity(root());
	}

	Entity Scene::createEntity(Entity parent){
		return parent.createChild();
	}

	void Scene::destroyEntity(Entity entity){
		entity.destroy();
	}
}