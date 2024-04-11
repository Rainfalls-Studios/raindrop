#ifndef __RAINDROP_CORE_SCENES_SCENE_INL_HPP__
#define __RAINDROP_CORE_SCENES_SCENE_INL_HPP__

#include "Scene.hpp"

namespace Raindrop::Core::Scenes{
	template<typename Component, typename... Args>
	Component& Scene::emplace(const EntityLUID& entity, Args&&... args){
		return registry::emplace<Component, Args...>(static_cast<entt::entity>(entity), std::forward<Args>(args)...);
	}

	template<typename Component>
	Component& Scene::get(const EntityLUID& entity){
		return registry::get<Component>(static_cast<entt::entity>(entity));
	}

	template<typename P, typename... Args>
	P* Scene::addProperty(Args&&... args){
		static_assert(std::is_base_of_v<Property, P>, "The custom property type has to be derived from Raindrop::SceneProperty (Raindrop::Core::Scenes::Property)");
		auto property = std::make_unique<P>(std::forward<Args>(args)...);
		addProperty(typeid(P), std::unique_ptr<Property>(static_cast<Property*>(property.release())));
		return getProperty<P>();
	}

	template<typename P>
	void Scene::removeProperty(){
		removeProperty(typeid(P));
	}

	template<typename P>
	P* Scene::getProperty(){
		static_assert(std::is_base_of_v<Property, P>, "The custom property type has to be derived from Raindrop::SceneProperty (Raindrop::Core::Scenes::Property)");
		return static_cast<P*>(getProperty(typeid(P)));
	}
}

#endif