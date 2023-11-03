#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_SCENE_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_SCENE_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/Scene.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UintTests::Core::Scene{
	class SceneMock : public ::Raindrop::Core::Scene::Scene{
		public:
			using ComponentID = ::Raindrop::Core::Scene::ComponentID;
			using EntityID = ::Raindrop::Core::Scene::EntityID;
			using ConstructorPtr = ::Raindrop::Core::Scene::ConstructorPtr;
			using DestructorPtr = ::Raindrop::Core::Scene::DestructorPtr;

			SceneMock(::Raindrop::Core::Scene::Context& context, uint32_t entityCount, uint32_t componentCount) : Scene(context, entityCount, componentCount){}

			MOCK_METHOD(uint32_t, maxEntityCount, (), (const));
			MOCK_METHOD(uint32_t, maxComponentCount, (), (const));
			MOCK_METHOD(uint32_t, usedEntityCount, (), (const));
			MOCK_METHOD(uint32_t, usedComponentCount, (), (const));

			MOCK_METHOD(ComponentID, registerComponent, (uint32_t, size_t, uint32_t, ConstructorPtr, DestructorPtr));
			MOCK_METHOD(void, unregisterComponent, (ComponentID));
			MOCK_METHOD(ComponentID, getComponentID, (size_t));
			MOCK_METHOD(bool, componentRegistred, (ComponentID));

			MOCK_METHOD(EntityID, createEntity, ());
			MOCK_METHOD(void, destroyEntity, (EntityID));

			MOCK_METHOD(bool, hasComponent, (EntityID, ComponentID));
			MOCK_METHOD(void*, getComponent, (EntityID, ComponentID));
			MOCK_METHOD(void*, createComponent, (EntityID, ComponentID));
			MOCK_METHOD(void, destroyComponent, (EntityID, ComponentID));

			MOCK_METHOD(EntityID, root, (), (const));
			MOCK_METHOD(const std::string&, name, (), (const));

			MOCK_METHOD(std::list<EntityID>&, componentEntities, (ComponentID));
	};
}

#endif