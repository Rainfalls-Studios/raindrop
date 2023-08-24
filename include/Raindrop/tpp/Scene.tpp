template<typename T>
const std::list<Entity>& Scene::filterEntitiesWithComponent() const{
	auto list = _scene->componentEntities<T>();
	return std::list<Entity>(list.begin(), list.end());
}