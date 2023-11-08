#ifndef __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	// TODO: add const methods
	// TODO: add operator overloads
	class ComponentManager{
		public:
			// TODO: refactor constructor
			ComponentManager(Context& context, std::size_t componentSize, std::size_t typeID, std::size_t size, ConstructorPtr constructor, DestructorPtr destructor);
			~ComponentManager();

			void* get(ComponentHandleID id);
			std::size_t size() const;

			ComponentHandleID createComponent();
			void destroyComponent(ComponentHandleID id);

			void* array();

			void addEntity(EntityID entity);
			void removeEntity(EntityID entity);
			std::list<EntityID>& entities();
			
			ConstructorPtr constructor();
			DestructorPtr destructor();

		private:
			Context& _context;

			// create a vector of 1byte data type.
			std::vector<char> _components;
			std::unordered_set<ComponentHandleID> _IDsPool;
			std::list<EntityID> _entities;
			std::list<ComponentHandleID> _usedHandles;
			
			ConstructorPtr _constructor;
			DestructorPtr _destructor;
			
			std::size_t _componentSize;
			std::size_t _size;
			std::size_t _typeID;
	};
}

#endif