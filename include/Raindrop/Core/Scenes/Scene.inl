#ifndef __RAINDROP_CORE_SCENES_SCENE_INL_HPP__
#define __RAINDROP_CORE_SCENES_SCENE_INL_HPP__

#include "Scene.hpp"

namespace Raindrop::Core::Scenes{
	template<typename Component, typename... Args>
	Component& Scene::emplace(const EntityLUID& entity, Args&&... args){
		return registry::emplace<Component>(static_cast<entt::entity>(entity), args...);
	}

	template<typename Component>
	Component& Scene::get(const EntityLUID& entity){
		return registry::get<Component>(static_cast<entt::entity>(entity));
	}
}

#endif