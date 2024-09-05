#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Core::Scenes{
	using EntityID = std::uint32_t;
	constexpr EntityID INVALID_ENTITY_ID = EntityID(~0UL);

	using Registry = entt::basic_registry<EntityID>;
}