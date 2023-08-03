#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Core::Scene{
	Entity::Entity(){
		reset();
	}

	Entity::Entity(EntityID id, Scene* scene) : _id{id}, _scene{scene}{}
	Entity::Entity(const Entity &other) : _id{other._id}, _scene{other._scene}{}
	Entity::~Entity(){}

	void Entity::reset(){
		_id = INVALID_ENTITY_ID;
		_scene = nullptr;
	}

	bool Entity::hasComponent(ComponentID component){
		return _scene->hasComponent(_id, component);
	}

	void* Entity::getComponent(ComponentID component){
		return _scene->getComponent(_id, component);
	}

	void* Entity::createComponent(ComponentID component){
		return _scene->createComponent(_id, component);
	}

	void Entity::destroyComponent(ComponentID component){
		_scene->destroyComponent(_id, component);
	}

	Entity::operator EntityID() const{
		return _id;
	}

	Components::Transform& Entity::transform(){
		return getComponent<Components::Transform>();
	}

	Components::Tag& Entity::tag(){
		return getComponent<Components::Tag>();
	}

	Components::Hierarchy& Entity::hierarchy(){
		return getComponent<Components::Hierarchy>();
	}

	Entity::Iterator Entity::begin(){
		auto &a = hierarchy();
		return Iterator(a.childs.begin(), _scene);
	}

	Entity::Iterator Entity::end(){
		return Iterator(hierarchy().childs.end(), _scene);
	}

	EntityID Entity::id() const{
		return _id;
	}

	Scene* Entity::scene() const{
		return _scene;
	}

	Entity Entity::createChild(){
		Entity child = Entity(_scene->createEntity(), _scene);
		hierarchy().childs.push_back(child);
		child.hierarchy().parent = _id;
		return child;
	}

	Entity::operator bool() const{
		return _id == INVALID_ENTITY_ID || _scene == nullptr;
	}

	bool Entity::operator==(const EntityID& other) const{
		return _id == other;
	}

	bool Entity::operator==(const Entity& other) const{
		return _id == other._id && _scene == other._scene;
	}

	bool Entity::operator!=(const Entity& other) const{
		return !(*this == other);
	}

	bool Entity::operator!=(const EntityID& other) const{
		return !(*this == other);
	}
}