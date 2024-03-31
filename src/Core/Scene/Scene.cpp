#include <Raindrop/Core/Scenes/Scene.hpp>
#include <Raindrop/Core/Scenes/Entity.hpp>

namespace Raindrop::Core::Scenes{
	EntityLUID Scene::create(){
		return static_cast<EntityLUID>(registry::create());
	}

	void Scene::destroy(const EntityLUID& entity){
		registry::destroy(static_cast<entt::entity>(entity));
	}
}

