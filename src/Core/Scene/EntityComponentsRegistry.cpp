#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>


namespace Raindrop::Core::Scene{
	EntityComponentsRegistry::EntityComponentsRegistry(Context& context, uint32_t entityCount, uint32_t componentCount) : _context{context}{
		_ids.resize(componentCount);
		for (auto &v : _ids){
			v.resize(entityCount);
			std::fill(v.begin(), v.end(), INVALID_COMPONENT_HANDLE_ID);
		}
	}

	EntityComponentsRegistry::~EntityComponentsRegistry(){}

	ComponentHandleID& EntityComponentsRegistry::get(EntityID entity, ComponentID component){
		checkEntityRange(component, entity);
		return _ids[component][entity];
	}

	const ComponentHandleID& EntityComponentsRegistry::get(EntityID entity, ComponentID component) const{
		checkEntityRange(component, entity);
		return _ids[component][entity];
	}

	void EntityComponentsRegistry::set(EntityID entity, ComponentID component, ComponentHandleID handle){
		checkEntityRange(component, entity);
		_ids[component][entity] = handle;
	}
	
	uint32_t EntityComponentsRegistry::componentCount() const{
		return _ids.size();
	}

	uint32_t EntityComponentsRegistry::entityCount() const{
		return _ids[0].size();
	}

	void EntityComponentsRegistry::checkComponentRange(ComponentID component) const{
		if (component >= _ids.size())
			throw std::out_of_range("invalid component ID");
	}

	void EntityComponentsRegistry::checkEntityRange(ComponentID component, EntityID entity) const{
		checkComponentRange(component);
		if (entity >= _ids[component].size())
			throw std::out_of_range("invalid entity ID");
	}

}