#include <Raindrop/Core/Scenes/Entity.hpp>
#include <Raindrop/Core/Scenes/Scene.hpp>

namespace Raindrop::Core::Scenes{
	Entity::Entity(Scene& scene, const EntityLUID& ID) : _scene{scene}, _ID{ID}{}
	Entity::Entity(const Entity& other) : _scene{other._scene}, _ID{other._ID}{}
	Entity::Entity(Scene& scene) : _scene{scene}, _ID{INVALID_ENTITY_LUID}{}

	Entity::~Entity(){}

	EntityLUID Entity::LUID() const{
		return _ID;
	}

	EntityGUID Entity::GUID() const{
		return EntityGUID(_ID, 0);
	}
}