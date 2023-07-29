#include <Raindrop/Core/Scene/Scene.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>
#include <Raindrop/Core/Scene/ComponentRegistry.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>
#include <Raindrop/Core/Scene/EntityManager.hpp>
#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>

#include <Raindrop/Core/Scene/Components/Camera.hpp>
#include <Raindrop/Core/Scene/Components/Hierarchy.hpp>
#include <Raindrop/Core/Scene/Components/Model.hpp>
#include <Raindrop/Core/Scene/Components/Tag.hpp>
#include <Raindrop/Core/Scene/Components/Transform.hpp>
#include <Raindrop/Core/Scene/Components/LightPoint.hpp>
#include <Raindrop/Core/Scene/Components/Spotlight.hpp>
#include <Raindrop/Core/Scene/Components/Sun.hpp>

namespace Raindrop::Core::Scene{
	Scene::Scene(EngineContext& context, uint32_t entityCount, uint32_t componentCount) : _context{context}{
		_root = INVALID_ENTITY_ID;

		el::Logger* customLogger = el::Loggers::getLogger("Engine.Core.Scene");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Core.Scene") << "Creating new scene...";
		_componentRegistry = std::make_unique<ComponentRegistry>(componentCount);
		_entityComponentsRegistry = std::make_unique<EntityComponentsRegistry>(entityCount, componentCount);
		_entityManager = std::make_unique<EntityManager>(entityCount);
		CLOG(INFO, "Engine.Core.Scene") << "Scene created with success !";

		registerComponent<Components::Tag>(entityCount);
		registerComponent<Components::Transform>(entityCount);
		registerComponent<Components::Hierarchy>(entityCount);
		registerComponent<Components::Camera>(1);
		registerComponent<Components::Model>(entityCount);
		registerComponent<Components::LightPoint>(entityCount);
		registerComponent<Components::Spotlight>(entityCount);
		registerComponent<Components::Sun>(entityCount);

		_root = createEntity();
	}

	Scene::~Scene(){
		CLOG(INFO, "Engine.Core.Scene") << "Destroying scene...";
		_componentRegistry.reset();
		_entityComponentsRegistry.reset();
		_entityManager.reset();
		CLOG(INFO, "Engine.Core.Scene") << "Scene destroyed with success !";
	}

	uint32_t Scene::maxEntityCount() const{
		return _entityManager->size();
	}

	uint32_t Scene::maxComponentCount() const{
		return _componentRegistry->size();
	}

	uint32_t Scene::usedEntityCount() const{
		return _entityManager->usedSize();
	}

	uint32_t Scene::usedComponentCount() const{
		return _componentRegistry->usedSize();
	}

	ComponentID Scene::registerComponent(uint32_t componentSize, size_t typeID, uint32_t maxCount, ConstructorPtr constructor, DestructorPtr destructor){
		return _componentRegistry->registerComponent(componentSize, typeID, maxCount, constructor, destructor);
	}
	
	void Scene::unregisterComponent(ComponentID id){
		_componentRegistry->unregisterComponent(id);
	}

	ComponentID Scene::getComponentID(size_t typeID){
		return _componentRegistry->getComponentID(typeID);
	}
	
	EntityID Scene::createEntity(){
		EntityID entity = _entityManager->createEntity();

		createComponent<Components::Transform>(entity);
		auto& hierarchy = createComponent<Components::Hierarchy>(entity);
		createComponent<Components::Tag>(entity);

		return entity;
	}

	void Scene::destroyEntity(EntityID ID){
		if (ID == _root){
			CLOG(WARNING, "Engine.Core.Scene") << "Cannot destroy the root entity of the scene";
			throw std::runtime_error("Cannot destroy root");
		}
		
		if (hasComponent<Components::Hierarchy>(ID)){
			auto& component = getComponent<Components::Hierarchy>(ID);
			
			for (auto &c : component.childs){
				destroyEntity(c);
			}

			if (component.parent != INVALID_ENTITY_ID){
				auto& parentComponent = getComponent<Components::Hierarchy>(component.parent);
				parentComponent.childs.remove(ID);
			}
		}

		for (uint32_t i=0; i<_entityComponentsRegistry->componentCount(); i++){
			auto& component = _entityComponentsRegistry->get(ID, i);
			if (component != INVALID_COMPONENT_HANDLE_ID){
				_componentRegistry->destroyHandle(i, component, ID);
			}
			component = INVALID_COMPONENT_HANDLE_ID;
		}
		
		_entityManager->destroyEntity(ID);
	}

	bool Scene::hasComponent(EntityID entity, ComponentID component){
		ComponentHandleID handleID = _entityComponentsRegistry->get(entity, component);
		return handleID != INVALID_COMPONENT_HANDLE_ID;
	}

	void* Scene::getComponent(EntityID entity, ComponentID component){
		ComponentHandleID handleID = _entityComponentsRegistry->get(entity, component);
		if (handleID == INVALID_COMPONENT_HANDLE_ID){
			throw std::runtime_error("the entity does not have this component");
		}
		return _componentRegistry->getComponent(component, handleID);
	}

	void* Scene::createComponent(EntityID entity, ComponentID component){
		ComponentHandleID handle = _componentRegistry->createHandle(component, entity);
		_entityComponentsRegistry->set(entity, component, handle);
		void* componentPtr = getComponent(entity, component);
		return componentPtr;
	}

	void Scene::destroyComponent(EntityID entity, ComponentID component){
		ComponentHandleID& handle = _entityComponentsRegistry->get(entity, component);
		_componentRegistry->destroyHandle(component, handle, entity);
		handle = INVALID_COMPONENT_HANDLE_ID;
	}

	bool Scene::componentRegistred(size_t typeID){
		return _componentRegistry->registred(typeID);
	}

	EntityID Scene::root() const{
		return _root;
	}

	std::list<EntityID>& Scene::componentEntities(ComponentID component){
		return _componentRegistry->getManager(component)->entities();
	}
}