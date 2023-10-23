#include <Raindrop/Core/Scene/EntityManager.hpp>

namespace Raindrop::Core::Scene{
	EntityManager::EntityManager(Context& context, uint32_t size) : _size{size}, _context{context}{
		for (uint32_t i=0; i<size; i++){
			_IDsPool.push(i);
		}
	}

	EntityManager::~EntityManager(){}

	EntityID EntityManager::createEntity(){
		EntityID id = _IDsPool.front();
		_IDsPool.pop();
		return id;
	}

	void EntityManager::destroyEntity(EntityID id){
		_IDsPool.push(id);
	}

	uint32_t EntityManager::size() const{
		return _size;
	}

	uint32_t EntityManager::usedSize() const{
		return _size - _IDsPool.size();
	}
}