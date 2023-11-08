#include <Raindrop/Core/Scene/ComponentManager.hpp>

namespace Raindrop::Core::Scene{
	ComponentManager::ComponentManager(Context& context, std::size_t componentSize, std::size_t typeID, std::size_t size, ConstructorPtr constructor, DestructorPtr destructor) : _context{context}, _componentSize{componentSize}, _typeID{typeID}, _size{size}, _constructor{constructor}, _destructor{destructor}{
		assert(componentSize != 0 && "the component size cannot be null");
		assert(size != 0 && "the component manager cannot have an null amount of component");
		
		_components.resize(size * componentSize);

		for (uint32_t i=0; i<size; i++){
			_IDsPool.insert(i);
		}
	}

	ComponentManager::~ComponentManager(){
		for (auto c : _usedHandles){
			_destructor(get(c));
		}

		_components.clear();
	}

	void* ComponentManager::get(ComponentHandleID id){
		std::size_t offset = id * _componentSize;
		if (offset > _components.size()) throw std::out_of_range("out of range of the allocated components");
		return static_cast<char*>(_components.data()) + offset;
	}

	std::size_t ComponentManager::size() const{
		return _size;
	}

	void* ComponentManager::array(){
		return static_cast<void*>(_components.data());
	}

	ComponentHandleID ComponentManager::createComponent(){
		auto it = _IDsPool.begin();
		ComponentHandleID id = *it;
		_IDsPool.erase(it);
		_constructor(get(id));
		_usedHandles.push_back(id);
		return id;
	}

	void ComponentManager::destroyComponent(ComponentHandleID id){
		if (_IDsPool.find(id) != _IDsPool.end()){
			throw std::runtime_error("cannot destroy twice the same component");
		}

		_IDsPool.insert(id);
		_usedHandles.remove(id);
		_destructor(get(id));
	}

	void ComponentManager::addEntity(EntityID entity){
		_entities.push_back(entity);
	}

	void ComponentManager::removeEntity(EntityID entity){
		auto it = std::find(_entities.begin(), _entities.end(), entity);
		if (it == _entities.end()){
			throw std::runtime_error("Cannot remove an unregistred entity");
		}
		_entities.erase(it);
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