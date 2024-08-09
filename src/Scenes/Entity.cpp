#include <Raindrop/Scenes/Entity.hpp>
#include <Raindrop/Scenes/Context.hpp>
#include <Raindrop/Scenes/Scene.hpp>

namespace Raindrop::Scenes{
	Entity::Entity() noexcept : 
		_scene{},
		_id{INVALID_ENTITY_ID}
	{}

	Entity::Entity(const std::shared_ptr<Scene>& scene, const EntityID& id) : 
		_scene{scene},
		_id{id}
	{}
	
	Entity::Entity(const Entity& other) : 
		_scene{other._scene},
		_id{other._id}
	{}

	Entity& Entity::operator=(const Entity& other){
		_scene = other._scene;
		_id = other._id;
		return *this;
	}

	Entity::Entity(Entity&& other) : 
		_scene{},
		_id{INVALID_ENTITY_ID}
	{
		swap(*this, other);
	}
	
	Entity& Entity::operator=(Entity& other){
		swap(*this, other);
		return *this;
	}

	void swap(Entity& A, Entity& B){
		std::swap(A._scene, B._scene);
		std::swap(A._id, B._id);
	}

	const EntityID& Entity::getID() const noexcept{
		return _id;
	}

	std::shared_ptr<Scene> Entity::getScene(){
		if (_scene.expired()) throw std::runtime_error("Invalid entity");
		return _scene.lock();
	}

	const std::shared_ptr<Scene> Entity::getScene() const{
		if (_scene.expired()) throw std::runtime_error("Invalid entity");
		return _scene.lock();
	}

	bool Entity::valid() const noexcept{
		return !_scene.expired() && _id != INVALID_ENTITY_ID;
	}

}