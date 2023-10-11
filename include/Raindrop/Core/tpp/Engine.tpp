template<typename T>
std::weak_ptr<T> Engine::loadOrGet(const std::filesystem::path& path){
	return std::static_pointer_cast<T>(loadOrGet(path).lock());
}

template<typename T, typename... Args>
void Engine::registerFactory(Args... args){
	registerFactory(std::static_pointer_cast<Asset::AssetFactory>(std::make_shared<T>(assetContext(), args...)), typeid(T).hash_code());
}

template<typename T>
void Engine::removeFactory(){
	removeFactory(typeid(T).hash_code());
}