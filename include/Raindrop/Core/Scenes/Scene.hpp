#pragma once

#include "common.hpp"
#include "Raindrop/pch.pch"
#include "Property.hpp"
#include "Raindrop/Core/Object.hpp"

namespace Raindrop::Core::Scenes{
	class Context;
	class Entity;

	class Scene : private Registry, public Object, public std::enable_shared_from_this<Scene>{
		public:
			Scene() noexcept;
			~Scene();

			Scene(Scene&& other);
			Scene& operator=(Scene&& other);

			void prepare(Context& context);
			void initialize();
			void release();

			static void swap(Scene& A, Scene& B);

			template<typename T, typename... Args>
			T& addProperty(Args&&... args){
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				if (hasProperty<T>()){
					throw std::runtime_error("Cannot add an already added property");
				}
				
				auto [it, success] = _properties.emplace(typeid(T), std::make_unique<T, Args...>(std::forward<Args>(args)...));
				return *static_cast<T*>(it->second.get());
			}

			template<typename T>
			T& addProperty(){
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				if (hasProperty<T>()){
					throw std::runtime_error("Cannot add an already added property");
				}
				
				std::unique_ptr<T> t;
				auto [it, success] = _properties.emplace(typeid(T), std::make_unique<T>());
				return *static_cast<T*>(it->second.get());
			}

			template<typename T>
			T* getProperty(){
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				auto it = _properties.find(typeid(T));
				if (it == _properties.end()){
					// _context->logger->warn("Could not find scene property \"{}\"", typeid(T).name());
					return nullptr;
				}

				return it->second.get();
			}

			template<typename T>
			const T* getProperty() const{
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				auto it = _properties.find(typeid(T));
				if (it == _properties.end()){
					// _context->logger->warn("Could not find scene property \"{}\"", typeid(T).name());
					return nullptr;
				}

				return it->second.get();
			}

			template<typename T>
			bool hasProperty() const noexcept{
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				return _properties.count(typeid(T)) == 1;
			}

			template<typename T>
			void removeProperty() noexcept{
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				_properties.erase(typeid(T));
			}

			Entity create();
			Entity create(const EntityID& hint);
			void destroy(const Entity& entity);

			Registry& getRegistry() noexcept;
			const Registry& getRegistry() const noexcept;

			template<typename... Types>
			void trim(){
				Registry::compact<Types...>();
			}

			template<typename... Types>
			void clear(){
				Registry::clear<Types...>();
			}

			template<typename Type>
			auto onConstruction(){
				return Registry::on_construct<Type>();
			}

			template<typename Type>
			auto onUpdate(){
				return Registry::on_update<Type>();
			}

			template<typename Type>
			auto onDestroy(){
				return Registry::on_destroy<Type>();
			}

			using Registry::view;
			using Registry::group;
			using Registry::group_if_exists;
			using Registry::owned;
			using Registry::sort;
			using Registry::orphan;

		private:
			Context* _context;
			std::unordered_map<std::type_index, std::unique_ptr<Property>> _properties;
	};
}