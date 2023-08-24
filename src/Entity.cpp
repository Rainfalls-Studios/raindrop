#include <Raindrop/Entity.hpp>

namespace Raindrop{
	Entity::Entity(const Core::Scene::Entity& entity) : _entity{entity}{}
	Entity::Entity(const Entity& other) : _entity{other._entity}{}
	Entity::Entity() : _entity{Core::Scene::INVALID_ENTITY_ID, nullptr}{}
	Entity::~Entity(){}
	
	bool Entity::operator==(const Entity& other) const{
		return _entity == other._entity;
	}

	bool Entity::operator!=(const Entity& other) const{
		return _entity != other._entity;
	}

	Entity& Entity::operator=(const Entity& other){
		_entity = other._entity;
		return *this;
	}

	Entity& Entity::operator=(const Core::Scene::Entity& entity){
		_entity = entity;
		return *this;
	}

	Entity Entity::createChild(){
		return Entity(_entity.createChild());
	}
	
	Entity::Iterator Entity::begin(){
		return Iterator(_entity.begin());
	}

	Entity::Iterator Entity::end(){
		return Iterator(_entity.end());
	}

	Entity Entity::parent(){
		// TODO
		// return Entity(_entity.parent());
	}

	const Entity Entity::parent() const{
		// TODO
		// return Entity(_entity.parent());
	}

	uint64_t Entity::UUID() const{
		// TODO
		// return _entity.tag().UUID;
	}
	
	std::string& Entity::tag(){
		return _entity.tag().name;
	}

	const std::string& Entity::tag() const{
		// TODO
		// return _entity.tag().name;
	}

	// ITERATOR

	Entity::Iterator::Iterator(value_type it) : _it{it}{}

	Entity::Iterator Entity::Iterator::operator++(){
		return Iterator(++_it);
	}

	Entity Entity::Iterator::operator*(){
		return Entity(Core::Scene::Entity(*_it));
	}

	bool Entity::Iterator::operator==(const Iterator& other){
		return _it == other._it;
	}

	bool Entity::Iterator::operator!=(const Iterator& other){
		return _it != other._it;
	}

	Entity Entity::Iterator::operator->(){
		return Entity(*_it);
	}

	void Entity::destroy(){
		_entity.scene()->destroyEntity(_entity.id());
	}
}