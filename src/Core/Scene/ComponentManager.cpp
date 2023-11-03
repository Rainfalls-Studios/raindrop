#include <Raindrop/Core/Scene/ComponentManager.hpp>

namespace Raindrop::Core::Scene{
	ComponentManager::ComponentManager(Context& context, std::size_t componentSize, std::size_t typeID, std::size_t size, ConstructorPtr constructor, DestructorPtr destructor) : _context{context}, _componentSize{componentSize}, _typeID{typeID}, _size{size}, _constructor{constructor}, _destructor{destructor}{
		assert(componentSize != 0 && "the component size cannot be null");
		assert(size != 0 && "the component manager cannot have an null amount of component");
		
		_components.resize(size * componentSize);

		for (uint32_t i=0; i<size; i++){
			_IDsPool.push(i);
		}
	}

	ComponentManager::~ComponentManager(){
		for (auto c : _usedHandles){
			_destructor(get(c));
		}

		_components.clear();
	}

	void* ComponentManager::get(ComponentHandleID id){
		return static_cast<char*>(_components.data()) + (id * _componentSize);
	}

	std::size_t ComponentManager::size() const{
		return _size;
	}

	void* ComponentManager::array(){
		return static_cast<void*>(_components.data());
	}

	ComponentHandleID ComponentManager::createComponent(){
		ComponentHandleID id = _IDsPool.front();
		_IDsPool.pop();
		_constructor(get(id));
		_usedHandles.push_back(id);
		return id;
	}

	void ComponentManager::destroyComponent(ComponentHandleID id){
		_IDsPool.push(id);
		_usedHandles.remove(id);
		_destructor(get(id));
	}

	void ComponentManager::addEntity(EntityID entity){
		_entities.push_back(entity);
	}

	void ComponentManager::removeEntity(EntityID entity){
		_entities.remove(entity);
	}

	std::list<EntityID>& ComponentManager::entities(){
		return _entities;
	}
	
	ConstructorPtr ComponentManager::constructor(){
		return _constructor;
	}

	DestructorPtr ComponentManager::destructor(){
		return _destructor;
	}
}