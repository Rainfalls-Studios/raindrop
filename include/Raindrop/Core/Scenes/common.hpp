#ifndef __RAINDROP_CORE_SCENES_COMMON_HPP__
#define __RAINDROP_CORE_SCENES_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Core::Scenes{
	class Scenes;
	class Scene;
	class Entity;
	
	// I suppose that 65.5 thousand scenes should be enough
	using SceneID = std::uint16_t;
	using EntityLUID = std::uint32_t;

	static constexpr EntityLUID INVALID_ENTITY_LUID = (EntityLUID)(~0);
	static constexpr SceneID INVALID_SCENE_ID = (SceneID)(~0);

	union EntityGUID{
		using GUID_t = std::uint64_t;
		static_assert(sizeof(GUID_t) >= sizeof(SceneID) + sizeof(EntityLUID), "The entity global unique indentifier has to be the same size has the entity local unique identifier plus the sceen identifier");
		
		static constexpr GUID_t INVALID_GUID = (GUID_t)(~0);

		struct {
			EntityLUID localID;
			SceneID sceneID;
		} components;

		GUID_t GUID;

		constexpr EntityGUID(const EntityLUID& localID, const SceneID& sceneID) : components{localID, sceneID}{}
		constexpr EntityGUID(const GUID_t& GUID) : GUID{GUID}{}
		constexpr EntityGUID(const EntityGUID& other) : GUID{other.GUID}{}
		constexpr EntityGUID() : GUID{INVALID_GUID}{}
	};

	static constexpr EntityGUID INVALID_ENTITY_GUID = EntityGUID(EntityGUID::INVALID_GUID);
}

#endif