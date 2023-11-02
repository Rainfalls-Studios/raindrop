#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_COMPONENT_MANAGER_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_COMPONENT_MANAGER_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/ComponentManager.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Scene{
	class ComponentManagerMock : public ::Raindrop::Core::Scene::ComponentManager{
		public:
			using ConstructorPtr = ::Raindrop::Core::Scene::ConstructorPtr;
			using DestructorPtr = ::Raindrop::Core::Scene::DestructorPtr;
			using ComponentHandleID = ::Raindrop::Core::Scene::ComponentHandleID;
			using EntityID = ::Raindrop::Core::Scene::EntityID;

			ComponentManagerMock(::Raindrop::Core::Scene::Context& context, std::size_t componentSize, std::size_t typeID, std::size_t size, ConstructorPtr constructor, DestructorPtr destructor) : ComponentManager(context, componentSize, typeID, size, constructor, destructor){}

			MOCK_METHOD(void*, get, (ComponentHandleID));
			MOCK_METHOD(std::size_t, size, (), (const));

			MOCK_METHOD(ComponentHandleID, createComponent, ());
			MOCK_METHOD(void, destroyComponent, (ComponentHandleID));

			MOCK_METHOD(void*, array, ());

			MOCK_METHOD(void, addEntity, (EntityID));
			MOCK_METHOD(void, removeEntity, (EntityID));
			MOCK_METHOD(std::list<EntityID>&, entities, ());

			MOCK_METHOD(ConstructorPtr, constructor, ());
			MOCK_METHOD(DestructorPtr, destructor, ());
	};
}

#endif