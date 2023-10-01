template<typename T>
void registerComponent(uint32_t size){
	registerComponent(sizeof(T), typeid(T).hash_code(), size, [](void* component){new (component) T();}, [](void* component){reinterpret_cast<T*>(component)->~T();});
}

template<typename T>
void unregisterComponent(){
	unregisterComponent(typeid(T).hash_code());
}

template<typename T>
T& getComponent(ComponentHandleID id){
	return *reinterpret_cast<T*>(getComponent(typeid(T).hash_code(), id));
}