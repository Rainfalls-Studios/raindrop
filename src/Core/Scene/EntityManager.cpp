#include <Raindrop/Core/Scene/EntityManager.hpp>

namespace Raindrop::Core::Scene{
	EntityManager::EntityManager(Context& context, uint32_t size) : _size{size}, _context{context}{
		for (uint32_t i=0; i<size; i++){
			_IDsPool.insert(i);
		}
	}

	EntityManager::~EntityManager(){}

	EntityID EntityManager::createEntity(){
		if (_IDsPool.empty()){
			throw std::runtime_error("Pool empty !");
		}

		auto it = _IDsPool.begin();
		EntityID id = *it;
		_IDsPool.erase(it);
		return id;
	}

	void EntityManager::destroyEntity(EntityID id){
		auto it = _IDsPool.find(id);
		if (it != _IDsPool.end())
			throw std::runtime_error("The entity indentifier has already been destroyed !");
		_IDsPool.insert(id);
	}

	uint32_t EntityManager::size() const{
		return _size;
	}

	uint32_t EntityManager::usedSize() const{
		return _size - _IDsPool.size();
	}
}