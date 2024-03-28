#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Core::Scene{
	EntityLUID Scene::create(){
		return static_cast<EntityLUID>(registry::create());
	}

	void Scene::destroy(const EntityLUID& entity){
		registry::destroy(static_cast<entt::entity>(entity));
	}
}

