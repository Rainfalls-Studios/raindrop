#ifndef __RAINDROP_SCENES_ENTITY_HPP__
#define __RAINDROP_SCENES_ENTITY_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Scenes{
	class Entity{
		public:
			Entity() noexcept;
			Entity(Scene& scene, const EntityID& id);
			
			Entity(const Entity& other);
			Entity& operator=(const Entity& other);

			Entity(Entity&& other);
			Entity& operator=(Entity& other);

			friend void swap(Entity& A, Entity& B);

			const EntityID& getID() const noexcept;

			template<typename T, typename... Args>
			T& add(Args&&... args){
				return getRegistry().emplace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& addOrReplace(Args&&... args){
				return getRegistry().emplace_or_replace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& getOrAdd(Args&&... args){
				return getRegistry().get_or_emplace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& replace(Args&&... args){
				return getRegistry().replace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Other>
			T& remove(){
				return getRegistry().erase<T, Other...>(_id);
			}

			template<typename... Types>
			bool hasAllOf() const{
				return getRegistry().all_of<Types...>(_id);
			}

			template<typename... Types>
			bool hasAnyOf() const{
				return getRegistry().any_of<Types...>(_id);
			}

			template<typename... Types>
			auto get(){
				return getRegistry().get<Types...>(_id);
			}

			template<typename... Types>
			auto get() const{
				return getRegistry().get<Types...>(_id);
			}

			template<typename... Types>
			auto tryGet(){
				return getRegistry().try_get<Types...>(_id);
			}

			template<typename... Types>
			auto tryGet() const{
				return getRegistry().try_get<Types...>(_id);
			}

			bool hasComponents() const{
				return getRegistry().orphan(_id);
			}

		private:
			Registry& getRegistry();
			const Registry& getRegistry() const;

			Scene* _scene;
			EntityID _id;
	};
}

#endif