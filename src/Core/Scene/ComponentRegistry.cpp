#include <Raindrop/Core/Scene/ComponentRegistry.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>
#include <Raindrop/Core/Scene/Context.hpp>

namespace Raindrop::Core::Scene{
	ComponentRegistry::ComponentRegistry(Context& context, uint32_t size) : _context{context}{
		_managers.resize(size);

		for (uint32_t i=0; i<size; i++){
			_IDsPool.insert(i);
		}
	}

	ComponentRegistry::~ComponentRegistry(){

	}

	ComponentID ComponentRegistry::registerComponent(uint32_t componentSize, size_t typeID, uint32_t size, ConstructorPtr constructor, DestructorPtr destructor){
		if (registred(typeID)){
			throw std::runtime_error("Cannot register the same component twice !");
		}

		ComponentID id = getNextID();
		_managers[id] = std::make_unique<ComponentManager>(_context, componentSize, typeID, size, constructor, destructor);
		_typeIDtoComponentID[typeID] = id;
		return id;
	}

	ComponentID ComponentRegistry::getNextID(){
		if (_IDsPool.empty()){
			_context.logger().error("Cannot register a new component. The component registry is full !");
			throw std::runtime_error("Cannot add another component !");
		}

		auto it = _IDsPool.begin();
		ComponentID id = *it;
		_IDsPool.erase(it);
		return id;
	}

	void* ComponentRegistry::getComponent(ComponentID component, ComponentHandleID handleID){
		checkRange(component);
		return _managers[component]->get(handleID);
	}

	void ComponentRegistry::unregisterComponent(size_t typeID){
		ComponentID component = getComponentID(typeID);

		checkRange(component);

		if (_IDsPool.find(component) != _IDsPool.end()){
			throw std::runtime_error("Cannot unregister twice the came component !");
		}

		_managers[component].reset();
		_typeIDtoComponentID.erase(typeID);
		_IDsPool.insert(component);
	}
	
	uint32_t ComponentRegistry::size() const{
		return _managers.size();
	}
	
	uint32_t ComponentRegistry::usedSize() const{
		return size() - _IDsPool.size();
	}

	ComponentID ComponentRegistry::getComponentID(size_t typeID){
		return _typeIDtoComponentID[typeID];
	}
	
	ComponentHandleID ComponentRegistry::createHandle(ComponentID component, EntityID entity){
		checkRange(component);
		auto& manager = _managers[component];
		ComponentHandleID handle = manager->createComponent();
		manager->addEntity(entity);
		return handle;
	}

	void ComponentRegistry::destroyHandle(ComponentID component, ComponentHandleID handle, EntityID entity){
		checkRange(component);
		auto& manager = _managers[component];
		manager->destroyComponent(handle);
		manager->removeEntity(entity);
	}

	bool ComponentRegistry::registred(size_t typeID){
		return _typeIDtoComponentID.find(typeID) != _typeIDtoComponentID.end();
	}

	ComponentManager* ComponentRegistry::getManager(ComponentID component){
		checkRange(component);
		return _managers[component].get();
	}

	void ComponentRegistry::checkRange(ComponentID component) const{
		if (component >= _managers.size())
			throw std::out_of_range("Invalid component ID");
	}

}