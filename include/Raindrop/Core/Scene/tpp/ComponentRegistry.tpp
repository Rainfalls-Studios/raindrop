template<typename T>
ComponentID ComponentRegistry::registerComponent(uint32_t size){
	return registerComponent(sizeof(T), typeid(T).hash_code(), size, [](void* component){new (component) T();}, [](void* component){reinterpret_cast<T*>(component)->~T();});
}

template<typename T>
void ComponentRegistry::unregisterComponent(){
	unregisterComponent(typeid(T).hash_code());
}

template<typename T>
T& ComponentRegistry::getComponent(ComponentHandleID id){
	return *reinterpret_cast<T*>(getComponent(typeid(T).hash_code(), id));
}