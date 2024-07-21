#ifndef __RAINDROP_SCENES_SCENE_HPP__
#define __RAINDROP_SCENES_SCENE_HPP__

#include "types.hpp"
#include "pch.pch"
#include "Property.hpp"

namespace Raindrop::Scenes{
	class Scene{
		public:
			Scene() noexcept;
			~Scene();

			void prepare(Context& context);
			void initialize();
			void release();

			template<typename T, typename... Args>
			T& addProperty(Args&&... args){
				static_assert(std::is_base_of<Property, T>::value, "The property type has to be derived from Raindrop::Scenes::Property");
				if (hasProperty<T>()){
					throw std::runtime_error("Cannot add an already added property");
				}
				
				auto [it, success] = _properties.emplace({typeid(T), std::make_unique<T, Args...>(std::forward<Args>(args)...)});
				return *it->second.get();
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


			entt::registry& getRegistry() noexcept;
			const entt::registry& getRegistry() const noexcept;

		private:
			Context* _context;
			entt::registry _registry;
			std::unordered_map<std::type_index, std::unique_ptr<Property>> _properties;
	};
}

#endif