#ifndef __RAINDROP_SCENES_TYPES_HPP__
#define __RAINDROP_SCENES_TYPES_HPP__

#include "pch.pch"

namespace Raindrop::Scenes{
	using EntityID = std::uint32_t;
	constexpr EntityID INVALID_ENTITY_ID = EntityID(~0UL);

	using Registry = entt::basic_registry<EntityID>;

	struct Context;
	class Scene;
	class Property;
	class Entity;
}

#endif