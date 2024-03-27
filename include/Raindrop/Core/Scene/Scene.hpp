#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include "common.hpp"
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

			EntityLUID create(){
				return static_cast<EntityLUID>(registry::create());
			}

			void destroy(const EntityLUID& entity){
				registry::destroy(static_cast<entt::entity>(entity));
			}

			using registry::emplace;

			template<typename Component, typename... Args>
			Component& emplace(const EntityLUID& entity, Args&&... args){
				return registry::emplace<Component>(static_cast<entt::entity>(entity), args...);
			}

			using registry::get;

			template<typename Component>
			Component& get(const EntityLUID& entity){
				return registry::get<Component>(static_cast<entt::entity>(entity));
			}
		
		private:
			Context& _context;
	};
}

#endif