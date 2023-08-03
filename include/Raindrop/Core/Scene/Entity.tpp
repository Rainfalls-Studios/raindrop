template<typename T>
bool Entity::hasComponent(){
	return _scene->hasComponent<T>(_id);
}

template<typename T>
T& Entity::getComponent(){
	return _scene->getComponent<T>(_id);
}

template<typename T>
T& Entity::createComponent(){
	return _scene->createComponent<T>(_id);
}

template<typename T>
void Entity::destroyComponent(){
	_scene->destroyComponent<T>(_id);
}