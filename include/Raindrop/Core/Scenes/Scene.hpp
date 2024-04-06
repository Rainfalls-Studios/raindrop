#ifndef __RAINDROP_CORE_SCENES_SCENE_HPP__
#define __RAINDROP_CORE_SCENES_SCENE_HPP__

#include "common.hpp"
#include "Property.hpp"
#include <entt/entt.hpp>

namespace Raindrop::Core::Scenes{
	static_assert(sizeof(entt::entity) == sizeof(EntityLUID), "The Entity local unique identifier has to be the same size has the EnTT entity handle");

	class Scene : public entt::registry{
		public:
			using registry = entt::registry;

			Scene() = default;
			~Scene() = default;

			using registry::clear;

			EntityLUID create();
			void destroy(const EntityLUID& entity);

			template<typename Component, typename... Args>
			Component& emplace(const EntityLUID& entity, Args&&... args);
			using registry::emplace;

			template<typename Component>
			Component& get(const EntityLUID& entity);
			using registry::get;

			void addProperty(const std::type_index& typeID, std::unique_ptr<Property>&& property);
			void removeProperty(const std::type_index& typeID);
			Property* getProperty(const std::type_index& typeID);

			template<typename P, typename... Args>
			void addProperty(Args&&... args);

			template<typename P>
			void removeProperty();

			template<typename P>
			P* getProperty();

		private:
			std::unordered_map<std::type_index, std::unique_ptr<Property>> _properties;
	};
}

#include "Scene.inl"

#endif