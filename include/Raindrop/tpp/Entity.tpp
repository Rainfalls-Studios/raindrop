template<typename T>
bool Entity::hasComponent() const{
	return _entity.hasComponent<T>();
}

template<typename T>
T& Entity::getComponent(){
	return _entity.getComponent<T>();
}

template<typename T>
const T& Entity::getComponent() const{
	return _entity.getComponent<T>();
}

template<typename T>
T& Entity::createComponent(T value){
	return _entity.createComponent<T>(value);
}

template<typename T>
void Entity::destroyComponent(){
	_entity.destroyComponent<T>();
}