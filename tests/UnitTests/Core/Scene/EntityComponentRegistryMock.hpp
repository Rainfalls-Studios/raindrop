#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_ENTITY_COMPONENT_REGISTRY_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_ENTITY_COMPONENT_REGISTRY_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Scene{
	class EntityComponentRegistryMock : public ::Raindrop::Core::Scene::EntityComponentRegistry{
		public:
			using ComponentHandleID = ::Raindrop::Core::Scene::ComponentHandleID;
			using EntityID = ::Raindrop::Core::Scene::EntityID;
			using ComponentID = ::Raindrop::Core::Scene::ComponentID;

			EntityComponentRegistryMock(::Raindrop::Core::Scene::Context& context, uint32_t entityCount, uint32_t componentCount) : EntityComponentManager(context, entityCount, componentCount){}

			MOCK_METHOD(ComponentHandleID&, get, (EntityID, ComponentID));
			MOCK_METHOD(const ComponentHandleID&, get, (EntityID, ComponentID), (const));
			MOCK_METHOD(void, set, (EntityID, ComponentID, ComopnentHandleID));

			MOCK_METHOD(uint32_t, componentCount, (), (const));
			MOCK_METHOD(uint32_t, entityCount, (), (const));
	};
}

#endif