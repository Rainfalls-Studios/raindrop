#ifndef __RAINDROP_CORE_SCENE_COMPONENT_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_REGISTRY_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class ComponentRegistry{
		public:
			ComponentRegistry(Context& context, uint32_t size);
			~ComponentRegistry();

			ComponentID registerComponent(uint32_t componentSize, size_t typeID, uint32_t size, ConstructorPtr constructor, DestructorPtr destructor);
			void unregisterComponent(ComponentID id);
			ComponentID getComponentID(size_t typeID);

			void* getComponent(ComponentID ID, ComponentHandleID handleID);

			uint32_t size() const;
			uint32_t usedSize() const;

			template<typename T> void registerComponent(uint32_t size);
			template<typename T> void unregisterComponent();
			template<typename T> T& getComponent(ComponentHandleID id);

			ComponentHandleID createHandle(ComponentID component, EntityID entity);
			void destroyHandle(ComponentID component, ComponentHandleID handle, EntityID entity);

			bool registred(size_t typeID);

			ComponentManager* getManager(ComponentID component);

		private:
			Context& _context;
			std::unordered_map<size_t, ComponentID> _typeIDtoComponentID;
			std::vector<std::unique_ptr<ComponentManager>> _managers;
			std::queue<ComponentID> _IDsPool;

			ComponentID getNextID();
	};

	#include <Raindrop/Core/Scene/tpp/ComponentRegistry.tpp>
}

#endif