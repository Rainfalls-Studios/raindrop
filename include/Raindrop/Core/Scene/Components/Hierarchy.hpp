#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_HIERARCHY_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_HIERARCHY_HPP__

#include <Raindrop/Core/Scene/Component.hpp>

namespace Raindrop::Core::Scene::Components{
	class Hierarchy : public Component{
		public:
			Hierarchy() = default;
			virtual ~Hierarchy() override = default;

			std::list<EntityID> childs;
			EntityID parent = INVALID_ENTITY_ID;
	};
}

#endif