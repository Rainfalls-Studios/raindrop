#ifndef __RAINDROP_CORE_SCENES_ENTIY_HPP__
#define __RAINDROP_CORE_SCENES_ENTIY_HPP__

#include "common.hpp"
#include "Scene.hpp"

namespace Raindrop::Core::Scenes{
	class Entity{
		public:
			Entity(Scene& scene, const EntityLUID& ID);
			Entity(const Entity& other);
			Entity(Scene& scene);
			~Entity();

			static Entity create(Scene& scene){
				return Entity(scene, scene.create());
			}

			EntityLUID LUID() const;
			EntityGUID GUID() const;

			template<typename Component, typename... Args>
			Component& emplace(Args&&... args){
				return _scene.emplace<Component>(_ID, args...);
			}

			template<typename Component>
			Component& get(){
				return _scene.get<Component>(_ID);
			}

		private:
			Scene& _scene;
			EntityLUID _ID;
	};
}

#endif