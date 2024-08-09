#ifndef __RAINDROP_UUID_REGISTRY_HPP__
#define __RAINDROP_UUID_REGISTRY_HPP__

#include "Object.hpp"
#include "UUID.hpp"
#include "pch.pch"
#include "Scenes/Entity.hpp"

namespace Raindrop{
	class UUIDRegistry{
		public:
			UUIDRegistry();
			~UUIDRegistry() = default;
			
			/**
			 * @brief Emplace a new object in the registry
			 * 
			 * @tparam T The type of object to emplace. MUST BE A DERIVED CLASS OF Raindrop::Object
			 * @tparam Args The constructor parameters types
			 * @param args The constructor parameters
			 * @return std::shared_ptr<T> 
			 */
			template<typename T, typename... Args>
			std::shared_ptr<T> emplace(Args&&... args){
				static_assert(std::is_base_of_v<Object, T>, "The object type MUST be derived from the Object class");

				std::shared_ptr<T> instance = std::make_unique<T>(std::forward<Args>(args)...);
				insert(std::static_pointer_cast<Object>(instance));

				return std::move(instance);
			}
			
			/**
			 * @brief Insert an already built instance of an object in the registry
			 * 
			 * @tparam T The type of object to emplace. MUST BE A DERIVED CLASS OF Raindrop::Object
			 * @param instance The actual instance
			 * @return UUID 
			 */
			template<typename T>
			UUID insert(const std::weak_ptr<T>& instance){
				static_assert(std::is_base_of_v<Object, T>, "The object type MUST be derived from the Object class");
				return insert(std::static_pointer_cast<Object>(instance));
			}

			/**
			 * @brief Insert an already built instance of an object in the registry
			 * 
			 * @param object The instance to insert
			 * @return The UUID given to this object
			 */
			UUID insert(const std::weak_ptr<Object>& object);

			/**
			 * @brief Get the object linked to the uuid
			 * 
			 * @param uuid The uuid potentialy linked to the wanted object
			 * @return the wanted instance or nullptr on invalid uuid
			 */
			std::shared_ptr<Object> get(const UUID& uuid);

			/**
			 * @brief Get the object linked to the uuid
			 * 
			 * @tparam T The object type required
			 * @param uuid the uuid potentialy linke to the wanted object
			 * @return the wanted instance or nullptr on invalid uuid or cast type
			 */
			template<typename T>
			std::shared_ptr<T> get(const UUID& uuid){
				static_assert(std::is_base_of_v<Object, T>, "The object type MUST be derived from the Object class");
				return std::dynamic_pointer_cast<T>(get(uuid));
			}

			/**
			 * @brief Get the object linked to the uuid
			 * 
			 * @tparam T The object type required (default as Raindrop::Object)
			 * @param uuid the uuid potentialy linke to the wanted object
			 * @return the wanted instance or nullptr on invalid uuid or cast type
			 */
			template<typename T = Object>
			std::shared_ptr<T> operator[](const UUID& uuid){
				return get<T>(uuid);
			}
			
			/**
			 * @brief Checks if an UUID is linked to anything
			 * 
			 * @param uuid the uuid to check
			 * @return true if the UUID is linked to an object instance, false otherwise
			 */
			bool exists(const UUID& uuid) const;

			/**
			 * @brief Removes the given uuid and it's linked data from the registry
			 * 
			 * @param uuid the uuid to remove
			 */
			void erase(const UUID& uuid);

			/**
			 * @brief Removes especialy en object
			 * 
			 * @param uuid the object's uuid
			 */
			void eraseObject(const UUID& uuid);

			/**
			 * @brief Removes especialy an entity
			 * @note Automaticaly handles by the scenes
			 * 
			 * @param uuid the entity's uuid
			 */
			void eraseEntity(const UUID& uuid);

			/**
			 * @brief Insert an entity into the registry. The registry should have the entit
			 * @note Automaticaly handles by the scenes
			 * 
			 * @param entity the entity to add
			 * @return UUID 
			 */
			UUID insertEntity(Scenes::Entity& entity);

			/**
			 * @brief Get the entity linked to the uuid
			 * 
			 * @param uuid The entity's uuid
			 * @return Scenes::Entity 
			 */
			Scenes::Entity& getEntity(const UUID& uuid);

		private:
			std::unordered_map<UUID, std::weak_ptr<Object>> _objects;
			std::unordered_map<UUID, Scenes::Entity> _entities;
	};
}

#endif