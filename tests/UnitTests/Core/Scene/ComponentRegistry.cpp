#include <common.hpp>
#include <Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/ComponentRegistry.hpp>

static constexpr std::size_t componentTypeCount = 150;
static constexpr std::size_t componentCount = 1000;
using Component = int;

// CONSTRUCTORS
TEST_F(CoreSceneContextFixture, ComponentRegistry_valid_constructor){
	EXPECT_NO_THROW(::Raindrop::Core::Scene::ComponentRegistry(context, componentTypeCount));
}

class CoreSceneComponentRegistryFixture : public CoreSceneContextFixture{
	public:
		CoreSceneComponentRegistryFixture() : registry{context, componentTypeCount}{}
		::Raindrop::Core::Scene::ComponentRegistry registry;
};

// METHODS
TEST_F(CoreSceneComponentRegistryFixture, size){
	EXPECT_EQ(registry.size(), componentTypeCount);
}

TEST_F(CoreSceneComponentRegistryFixture, usedSize){
	EXPECT_EQ(registry.usedSize(), 0);
}

TEST_F(CoreSceneComponentRegistryFixture, valid_componentRegistration){
	::Raindrop::Core::Scene::ComponentID ID;
	EXPECT_NO_THROW(ID = registry.registerComponent(
		sizeof(Component),
		typeid(Component).hash_code(),
		componentCount,
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;},
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;})
	);

	EXPECT_NE(ID, ::Raindrop::Core::Scene::INVALID_COMPONENT_ID);
	EXPECT_TRUE(registry.registred(typeid(Component).hash_code()));
}

TEST_F(CoreSceneComponentRegistryFixture, invalid_doubleComponentRegistration){
	EXPECT_NO_THROW(registry.registerComponent(
		sizeof(Component),
		typeid(Component).hash_code(),
		componentCount,
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;},
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;})
	);

	EXPECT_ANY_THROW(registry.registerComponent(
		sizeof(Component),
		typeid(Component).hash_code(),
		componentCount,
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;},
		[](void* arg){*reinterpret_cast<Component*>(arg) = 0;})
	);
}

class ComponentRegistryComponentFixture : public CoreSceneComponentRegistryFixture{
	public:
		virtual void SetUp() override{
			CoreSceneComponentRegistryFixture::SetUp();
			component = registry.registerComponent<Component>(componentCount);
		}

		::Raindrop::Core::Scene::ComponentID component;
};

TEST_F(ComponentRegistryComponentFixture, valid_unregister){
	EXPECT_NO_THROW(registry.unregisterComponent(typeid(Component).hash_code()));
	EXPECT_FALSE(registry.registred(typeid(Component).hash_code()));
}

TEST_F(ComponentRegistryComponentFixture, invalid_doubleUnregistring){
	EXPECT_NO_THROW(registry.unregisterComponent(typeid(Component).hash_code()));
	EXPECT_FALSE(registry.registred(typeid(Component).hash_code()));
	EXPECT_ANY_THROW(registry.unregisterComponent(typeid(Component).hash_code()));
}

TEST_F(ComponentRegistryComponentFixture, valid_getComponentBegin){
	EXPECT_NE(registry.getComponent(component, 0), nullptr);
}

TEST_F(ComponentRegistryComponentFixture, valid_getComponentMid){
	EXPECT_NE(registry.getComponent(component, componentCount / 2), nullptr);
}

TEST_F(ComponentRegistryComponentFixture, valid_getComponentEnd){
	EXPECT_NE(registry.getComponent(component, componentCount - 1), nullptr);
}

TEST_F(ComponentRegistryComponentFixture, invalid_getComponent){
	EXPECT_THROW(registry.getComponent(component, componentCount * 2), std::out_of_range);
}

TEST_F(ComponentRegistryComponentFixture, valid_handleCreation){
	::Raindrop::Core::Scene::ComponentHandleID id;
	EXPECT_NO_THROW(id = registry.createHandle(component, 0));
	EXPECT_NE(id, ::Raindrop::Core::Scene::INVALID_COMPONENT_HANDLE_ID);
}

TEST_F(ComponentRegistryComponentFixture, valid_handleDestruction){
	static constexpr ::Raindrop::Core::Scene::EntityID entity = 0;

	::Raindrop::Core::Scene::ComponentHandleID id = registry.createHandle(component, entity);
	EXPECT_NO_THROW(registry.destroyHandle(component, id, entity));
}

TEST_F(ComponentRegistryComponentFixture, getManager){
	EXPECT_NE(registry.getManager(component), nullptr);
}