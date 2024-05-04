#include <Raindrop/Core/Scenes/Scene.hpp>

namespace Raindrop::Core::Scenes{
	EntityLUID Scene::create(){
		return static_cast<EntityLUID>(registry::create());
	}

	void Scene::destroy(const EntityLUID& entity){
		registry::destroy(static_cast<entt::entity>(entity));
	}

	Property* Scene::addProperty(const std::type_index& typeID, std::unique_ptr<Property>&& property){
		_properties[typeID] = std::move(property);
		return getProperty(typeID);
	}

	void Scene::removeProperty(const std::type_index& typeID){
		_properties.erase(typeID);
	}

	Property* Scene::getProperty(const std::type_index& typeID){
		auto it = _properties.find(typeID);
		if (it == _properties.end()){
			return nullptr;
		}
		return it->second.get();
	}
}

