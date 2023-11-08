#include <common.hpp>
#include <Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/EntityManager.hpp>

static constexpr std::size_t entityCount = 150;

TEST_F(CoreSceneContextFixture, valid_EntityManager_constructor){
	EXPECT_NO_THROW(::Raindrop::Core::Scene::EntityManager(context, entityCount));
}

class CoreSceneEntityManagerFixture : public CoreSceneContextFixture{
	public:
		CoreSceneEntityManagerFixture() : manager(context, entityCount){}
		::Raindrop::Core::Scene::EntityManager manager;
};

TEST_F(CoreSceneEntityManagerFixture, size){
	EXPECT_EQ(manager.size(), entityCount);
}

TEST_F(CoreSceneEntityManagerFixture, usedSsize){
	EXPECT_EQ(manager.usedSize(), 0);
}

TEST_F(CoreSceneEntityManagerFixture, valid_entityCreation){
	::Raindrop::Core::Scene::EntityID entity;
	EXPECT_NO_THROW(entity = manager.createEntity());
	EXPECT_NE(entity, ::Raindrop::Core::Scene::INVALID_ENTITY_ID);
}

class CoreSceneEntityManagerFixtureEntity : public CoreSceneEntityManagerFixture{
	public:
		virtual void SetUp() override{
			CoreSceneEntityManagerFixture::SetUp();
			entity = manager.createEntity();
		}

		::Raindrop::Core::Scene::EntityID entity;
};

TEST_F(CoreSceneEntityManagerFixtureEntity, valid_destroy){
	EXPECT_NO_THROW(manager.destroyEntity(entity));
}

TEST_F(CoreSceneEntityManagerFixtureEntity, invalid_doubleDestroy){
	EXPECT_NO_THROW(manager.destroyEntity(entity));
	EXPECT_ANY_THROW(manager.destroyEntity(entity));
}

class CoreSceneMinimalEntityManagerFixture : public CoreSceneContextFixture{
	public:
		CoreSceneMinimalEntityManagerFixture() : manager(context, 2){}
		::Raindrop::Core::Scene::EntityManager manager;
};

TEST_F(CoreSceneMinimalEntityManagerFixture, valid_entityCreation){
	EXPECT_NO_THROW(manager.createEntity());
	EXPECT_NO_THROW(manager.createEntity());
}

TEST_F(CoreSceneMinimalEntityManagerFixture, invalid_entityCreation_outOfSpace){
	EXPECT_NO_THROW(manager.createEntity());
	EXPECT_NO_THROW(manager.createEntity());
	EXPECT_ANY_THROW(manager.createEntity());
}