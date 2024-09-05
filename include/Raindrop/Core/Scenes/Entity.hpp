#pragma once

#include "Raindrop/pch.pch"
#include "Scene.hpp"

namespace Raindrop::Core::Scenes{
	class Entity : public Object{
		public:
			Entity() noexcept;
			Entity(const std::shared_ptr<Scene>& scene, const EntityID& id);
			
			Entity(const Entity& other);
			Entity& operator=(const Entity& other);

			Entity(Entity&& other);
			Entity& operator=(Entity& other);

			friend void swap(Entity& A, Entity& B);

			const EntityID& getID() const noexcept;

			std::shared_ptr<Scene> getScene();
			const std::shared_ptr<Scene> getScene() const;

			bool valid() const noexcept;

			template<typename T, typename... Args>
			T& add(Args&&... args){
				return getScene()->emplace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& addOrReplace(Args&&... args){
				return getScene()->emplace_or_replace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& getOrAdd(Args&&... args){
				return getScene()->get_or_emplace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& replace(Args&&... args){
				return getScene()->replace<T, Args...>(_id, std::forward<Args>(args)...);
			}

			template<typename T, typename... Other>
			T& remove(){
				return getScene()->erase<T, Other...>(_id);
			}

			template<typename... Types>
			bool hasAllOf() const{
				return getScene()->all_of<Types...>(_id);
			}

			template<typename... Types>
			bool hasAnyOf() const{
				return getScene()->any_of<Types...>(_id);
			}

			template<typename... Types>
			auto get(){
				return getScene()->get<Types...>(_id);
			}

			template<typename... Types>
			auto get() const{
				return getScene()->get<Types...>(_id);
			}

			template<typename... Types>
			auto tryGet(){
				return getScene()->try_get<Types...>(_id);
			}

			template<typename... Types>
			auto tryGet() const{
				return getScene()->try_get<Types...>(_id);
			}

			bool hasComponents() const{
				return getScene()->orphan(_id);
			}

		private:
			std::weak_ptr<Scene> _scene;
			EntityID _id;
	};
}