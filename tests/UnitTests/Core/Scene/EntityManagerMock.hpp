#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_ENTITY_MANAGER_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_ENTITY_MANAGER_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/EntityManager.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Scene{
	class EntityManagerMock : public ::Raindrop::Core::Scene::EntityManager{
		public:
			using EntityID = ::Raindrop::Core::Scene::EntityID;
			EntityManagerMock(::Raindrop::Core::Scene::Context& context, uint32_t size) : EntityManager(context, size){}

			MOCK_METHOD(EntityID, createEntity, ());
			MOCK_METHOD(void, destroyEntity, (EntityID));

			MOCK_METHOD(uint32_t, size, (), (const));
			MOCK_METHOD(uint32_t, usedSize, (), (const));
	};
}

#endif