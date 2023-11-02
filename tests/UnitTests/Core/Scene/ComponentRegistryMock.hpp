#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_COMPONENT_REGISTRY_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_COMPONENT_REGISTRY_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/ComponentRegistry.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Scene{
	class ComponentRegistryMock : public ::Raindrop::Core::Scene::ComponentRegistry{
		public:
			using ComponentID = ::Raindrop::Core::Scene::ComponentID;
			using ComponentHandleID = ::Raindrop::Core::Scene::ComponentHandleID;
			using EntityID = ::Raindrop::Core::Scene::EntityID;
			using ConstructorPtr = ::Raindrop::Core::Scene::ConstructorPtr;
			using DestructorPtr = ::Raindrop::Core::Scene::DestructorPtr;

			ComponentRegistryMock(::Raindrop::Core::Scene::Context& context, uint32_t size) : ComponentRegistry(context, size){}

			MOCK_METHOD(ComponentID, registerComponent, (uint32_t, size_t, size_t, ConstructorPtr, DestructorPtr));
			MOCK_METHOD(void, unregisterComponent, (ComponentID));
			MOCK_METHOD(ComponentID, getComponentID, (size_t));

			MOCK_METHOD(void*, getComponent, (ComponentID, ComponentHandleID));

			MOCK_METHOD(uint32_t, size, (), (const));
			MOCK_METHOD(uint32_t, usedSize, (), (const));

			MOCK_METHOD(ComponentHandleID, createHandle, (ComponentID, EntityID));
			MOCK_METHOD(void, destroyHandle, (ComponentID, ComponentHandleID, EntityID));

			MOCK_METHOD(bool, registred, (size_t));
			MOCK_METHOD(::Raindrop::Core::Scene::ComponentManager*, getManager, (ComponentID));
	};
}

#endif