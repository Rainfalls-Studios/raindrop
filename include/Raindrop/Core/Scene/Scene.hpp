#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include "common.hpp"
#include "Entity.hpp"

#include <entt/entt.hpp>

namespace Raindrop::Core::Scene{
	static_assert(sizeof(entt::entity) == sizeof(EntityLUID), "The Entity local unique identifier has to be the same size has the EnTT entity handle");

	class Scene : public entt::registry{
		public:
			using registry = entt::registry;

			Scene(Context& context);
			~Scene();

			template<typename... Components>
			void clear(){
				registry::clear<Components...>();
			}

			Entity create(){
				return Entity(*this, static_cast<EntityLUID>(registry::create()));
			}

			void destroy(const Entity& entity){
				registry::destroy(static_cast<entt::entity>(entity.LUID()));
			}


			using registry::emplace;

			template<typename Component, typename... Args>
			Component& emplace(const Entity& entity, Args&&... args){
				return registry::emplace<Component>(static_cast<entt::entity>(entity.LUID()), args...);
			}
			

			using registry::get;

			template<typename Component>
			Component& get(const Entity& entity){
				return registry::get<Component>(static_cast<entt::entity>(entity.LUID()));
			}
		
		private:
			Context& _context;
	};
}

#endif