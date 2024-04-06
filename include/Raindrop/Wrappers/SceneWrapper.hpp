#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include "common.hpp"
#include "../Core/Scenes/common.hpp"
#include "../Core/Scenes/Scene.hpp"
#include "../Utils/Wrapper.hpp"

namespace Raindrop::Wrappers{
	class SceneWrapper : private Utils::Wrapper<Context, Core::Scenes::Scene, Core::Scenes::SceneID, Core::Scenes::INVALID_SCENE_ID>{
		public:
			using SceneID = Core::Scenes::SceneID;
			using Scene = Core::Scenes::Scene;
			using Property = Core::Scenes::Property;
			using EntityLUID = Core::Scenes::EntityLUID;

			using Wrapper::Wrapper;
			using Wrapper::operator=;
			using Wrapper::operator->;
			using Wrapper::ID;

			virtual ~SceneWrapper() override = default;

			virtual Scene& get() override;
			virtual const Scene& get() const override;

			inline void clear(){
				get().clear();
			}

			inline EntityLUID create(){
				return get().create();
			}

			inline void destroy(const EntityLUID& ID){
				get().destroy(ID);
			}

			template<typename Component, typename... Args>
			Component& emplaceComponent(const EntityLUID& entity, Args&&... args){
				return get().emplace<Component, Args...>(entity, std::forward<Args>(args)...);
			}

			template<typename Component>
			Component& getComponent(const EntityLUID& entity){
				return get().get<Component>(entity);
			}

			inline void addProperty(const std::type_index& typeID, std::unique_ptr<Property>&& property){
				get().addProperty(typeID, std::move(property));
			}

			inline void removeProperty(const std::type_index& typeID){
				get().removeProperty(typeID);
			}

			inline Property* getProperty(const std::type_index& typeID){
				return get().getProperty(typeID);
			}

			template<typename P, typename... Args>
			inline void addProperty(Args&&... args){
				get().addProperty<P, Args...>(std::forward<Args>(args)...);
			}

			template<typename P>
			inline void removeProperty(){
				get().removeProperty<P>();
			}

			template<typename P>
			inline P* getProperty(){
				return get().getProperty<P>();
			}
	};
}

#endif