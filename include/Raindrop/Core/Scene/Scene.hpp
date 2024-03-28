#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include "common.hpp"
#include <entt/entt.hpp>

namespace Raindrop::Core::Scene{
	static_assert(sizeof(entt::entity) == sizeof(EntityLUID), "The Entity local unique identifier has to be the same size has the EnTT entity handle");

	class Scene : public entt::registry{
		public:
			using registry = entt::registry;

			Scene() = default;
			virtual ~Scene() = default;

			using registry::clear;

			EntityLUID create();
			void destroy(const EntityLUID& entity);

			template<typename Component, typename... Args>
			Component& emplace(const EntityLUID& entity, Args&&... args);
			using registry::emplace;

			template<typename Component>
			Component& get(const EntityLUID& entity);
			using registry::get;
	};
}

#include "Scene.inl"

#endif