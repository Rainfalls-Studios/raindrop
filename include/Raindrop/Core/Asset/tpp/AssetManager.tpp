template<typename T, typename... Args>
void AssetManager::registerFactory(Args... args){
	static_assert(std::is_base_of<AssetFactory, T>::value, "");
	registerFactory(std::static_pointer_cast<AssetFactory>(std::make_shared<T>(*_context, args...)), typeid(T).hash_code());
}
template<typename T>
void AssetManager::removeFactory(){
	static_assert(std::is_base_of<AssetFactory, T>::value, "");
	removeFactory(typeid(T).hash_code());
}
template<typename T>
std::weak_ptr<T> AssetManager::loadOrGet(const std::filesystem::path& path){
	static_assert(std::is_base_of<Asset, T>::value, "");
	return std::static_pointer_cast<T>(loadOrGet(path).lock());
}