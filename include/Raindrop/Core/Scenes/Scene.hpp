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

			/**
			 * @brief Removes every entities and components
			 * 
			 */
			using registry::clear;

			/**
			 * @brief Creates an entity handle
			 * 
			 * @return EntityLUID 
			 */
			EntityLUID create();

			/**
			 * @brief Destroyes an entity

			 * @param entity The entity to destroy
			 */
			void destroy(const EntityLUID& entity);

			/**
			 * @brief Add a component to an entity
			 * 
			 * @tparam Component The component type
			 * @tparam Args The component construction arguments types
			 * @param entity The entity 
			 * @param args The component construction arguments
			 * @return Component& 
			 */
			template<typename Component, typename... Args>
			Component& emplace(const EntityLUID& entity, Args&&... args);
			using registry::emplace;

			/**
			 * @brief Returns en entity component
			 * 
			 * @tparam Component The component type
			 * @param entity The target entity
			 * @return Component& 
			 */
			template<typename Component>
			Component& get(const EntityLUID& entity);
			using registry::get;

			/**
			 * @brief Adds a custom property to the scene
			 * 
			 * @param typeID The property type index
			 * @param property The property itself
			 * @return A pointer to the property
			 */
			Property* addProperty(const std::type_index& typeID, std::unique_ptr<Property>&& property);

			/**
			 * @brief Removes a property from the scene
			 * 
			 * @param typeID The property type index
			 */
			void removeProperty(const std::type_index& typeID);

			/**
			 * @brief Get a scene property pointer. If the property has not been added to the scene, this method returns nullptr
			 * 
			 * @param typeID The property type index
			 * @return Property* or nullptr
			 */
			Property* getProperty(const std::type_index& typeID);

			/**
			 * @brief Adds a custom property to the scene
			 * 
			 * @tparam P The property type
			 * @tparam Args The property custructor arguments types
			 * @param args the The property custructor arguments
			 * @return A pointer to the newly created property
			 */
			template<typename P, typename... Args>
			P* addProperty(Args&&... args);

			/**
			 * @brief Removes a property from the scene
			 * 
			 * @tparam P The property type
			 */
			template<typename P>
			void removeProperty();

			/**
			 * @brief Get a scene property pointer. If the property has not been added to the scene, this method returns nullptr
			 * 
			 * @tparam P The property type
			 * @return a pointer to the property, nullptr if the property has not been registred
			 */
			template<typename P>
			P* getProperty();

		private:
			std::unordered_map<std::type_index, std::unique_ptr<Property>> _properties;
	};
}

#include "Scene.inl"

#endif