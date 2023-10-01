template<typename T, typename... Args>
void registerFactory(Args... args){
	registerFactory(std::static_pointer_cast<AssetFactory>(std::make_shared<T>(*_context, args...)), typeid(T).hash_code());
}
template<typename T>
void removeFactory(){
	removeFactory(typeid(T).hash_code());
}
template<typename T>
std::weak_ptr<T> loadOrGet(const std::filesystem::path& path){
	return std::static_pointer_cast<T>(loadOrGet(path).lock());
}