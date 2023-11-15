template<typename T, typename... Args>
std::shared_ptr<T> AssetManager::registerFactory(Args... args){
	static_assert(std::is_base_of<AssetFactory, T>::value, "");
	std::shared_ptr<T> factory = std::make_shared<T>(args...);
	registerFactory(std::static_pointer_cast<AssetFactory>(factory));
	return factory;
}
template<typename T>
std::weak_ptr<T> AssetManager::loadOrGet(const std::filesystem::path& path){
	static_assert(std::is_base_of<Asset, T>::value, "");
	return std::static_pointer_cast<T>(loadOrGet(path).lock());
}