template<typename T>
std::list<Entity> Scene::filterEntitiesWithComponent() const{
	auto list = _scene->componentEntities<T>();
	std::list<Entity> entityList;
	for (const auto& e : list){
		entityList.push_back(Core::Scene::Entity(e, _scene.get()));
	}
	return entityList;
}