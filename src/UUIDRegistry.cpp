#include <Raindrop/UUIDRegistry.hpp>
#include <Raindrop/Scenes/Entity.hpp>

namespace Raindrop{
	UUIDRegistry::UUIDRegistry() : 
		_objects{},
		_entities{}
	{}

	UUID UUIDRegistry::insert(const std::weak_ptr<Object>& object){
		auto ref = object.lock();
		if (!ref) return UUID();


		UUID uuid = UUID::Generate();
		_objects[uuid] = object;
		ref->_uuid = uuid;
		return uuid;
	}

	std::shared_ptr<Object> UUIDRegistry::get(const UUID& uuid){
		auto it = _objects.find(uuid);

		if (it != _objects.end()){
			auto& ref = it->second;

			if (!ref.expired()){
				return ref.lock();
			}

			// if the reference has been expired, there is no need to keep the uuid in the map
			_objects.erase(it);
		}

		return {};
	}
	
	bool UUIDRegistry::exists(const UUID& uuid) const{
		return (_objects.count(uuid) == 1) || (_entities.count(uuid) == 1);
	}

	void UUIDRegistry::erase(const UUID& uuid){
		eraseObject(uuid);
		eraseEntity(uuid);
	}

	void UUIDRegistry::eraseObject(const UUID& uuid){
		_objects.erase(uuid);
	}

	void UUIDRegistry::eraseEntity(const UUID& uuid){
		_entities.erase(uuid);
	}

	UUID UUIDRegistry::insertEntity(Scenes::Entity& entity){
		if (!entity.valid()) return UUID();

		UUID uuid = UUID::Generate();
		entity._uuid = uuid;

		_entities[uuid] = entity;
		return uuid;
	}

	Scenes::Entity& UUIDRegistry::getEntity(const UUID& uuid){
		static Scenes::Entity DEFAULT;

		auto it = _entities.find(uuid);

		if (it == _entities.end()){
			return DEFAULT;
		}

		Scenes::Entity& entity = it->second;
		
		if (entity.valid()){
			return entity;
		}

		_entities.erase(it);
		return DEFAULT;
	}
}