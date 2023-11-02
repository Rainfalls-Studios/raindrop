#ifndef __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class EntityManager{
		public:
			EntityManager(Context& context, uint32_t size);
			~EntityManager();

			EntityID createEntity();
			void destroyEntity(EntityID id);

			uint32_t size() const;
			uint32_t usedSize() const;

		private:
			Context& _context;
			std::queue<EntityID> _IDsPool;
			uint32_t _size;
	};
}

#endif