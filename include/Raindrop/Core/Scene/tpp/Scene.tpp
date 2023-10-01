
template<typename T>
ComponentID Scene::getComponentID(){
	return getComponentID(typeid(T).hash_code());
}

template<typename T, typename... Args>
ComponentID Scene::registerComponent(uint32_t maxCount, Args&&... args){
	return registerComponent(sizeof(T), typeid(T).hash_code(), maxCount, [&args...](void* component){new (component) T(args...);}, [](void* component){reinterpret_cast<T*>(component)->~T();});
}

template<typename T>
void Scene::unregisterComponent(){
	unregisterComponent(getComponentID<T>());
}

template<typename T>
bool Scene::componentRegistred(){
	return componentRegistred(getComponentID<T>());
}

template<typename T>
bool Scene::hasComponent(EntityID entity){
	return hasComponent(entity, getComponentID<T>());
}

template<typename T>
T& Scene::getComponent(EntityID entity){
	return *reinterpret_cast<T*>(getComponent(entity, getComponentID<T>()));
}

template<typename T>
T& Scene::createComponent(EntityID entity){
	return *reinterpret_cast<T*>(createComponent(entity, getComponentID<T>()));
}

template<typename T>
void Scene::destroyComponent(EntityID entity){
	destroyComponent(entity, getComponentID<T>());
}

template<typename T>
std::list<EntityID>& Scene::componentEntities(){
	return componentEntities(getComponentID<T>());
}
