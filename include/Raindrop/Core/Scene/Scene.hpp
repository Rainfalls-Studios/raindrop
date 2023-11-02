#ifndef __RAINDROP_CORE_SCENE_SCENE_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	// TODO: replace uint32 and size_t by std::size_t
	class Scene{
		public:
			Scene(Context& context, uint32_t entityCount = 1024, uint32_t componentCount = 128);
			~Scene();

			uint32_t maxEntityCount() const;
			uint32_t maxComponentCount() const;
			uint32_t usedEntityCount() const;
			uint32_t usedComponentCount() const;

			ComponentID registerComponent(uint32_t componentSize, size_t typeID, uint32_t maxCount, ConstructorPtr constructor, DestructorPtr destructor);
			void unregisterComponent(ComponentID id);
			ComponentID getComponentID(size_t typeID);
			bool componentRegistred(size_t typeID);

			EntityID createEntity();
			void destroyEntity(EntityID ID);

			bool hasComponent(EntityID entity, ComponentID component);
			void* getComponent(EntityID entity, ComponentID component);
			void* createComponent(EntityID entity, ComponentID component);
			void destroyComponent(EntityID entity, ComponentID component);

			template<typename T> ComponentID getComponentID();

			template<typename T, typename... Args> ComponentID registerComponent(uint32_t maxCount, Args&&... args);
			template<typename T> void unregisterComponent();
			template<typename T> bool componentRegistred();
			template<typename T> bool hasComponent(EntityID entity);
			template<typename T> T& getComponent(EntityID entity);
			template<typename T> T& createComponent(EntityID entity);
			template<typename T> void destroyComponent(EntityID entity);

			EntityID root() const;
			const std::string& name() const;

			std::list<EntityID>& componentEntities(ComponentID component);	

			template<typename T> std::list<EntityID>& componentEntities();

		private:
			Context& _context;
			std::unique_ptr<ComponentRegistry> _componentRegistry;
			std::unique_ptr<EntityComponentsRegistry> _entityComponentsRegistry;
			std::unique_ptr<EntityManager> _entityManager;

			EntityID _root;
			std::string _name;

			void registerComponents();
	};

	#include <Raindrop/Core/Scene/tpp/Scene.tpp>
}

#endif