#include <common.hpp>
#include <Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/EntityManager.hpp>

static constexpr std::size_t entityCount = 150;

namespace Raindrop::UnitTests::Core::Scene{
	TEST_F(ContextFixture, valid_EntityManager_constructor){
		EXPECT_NO_THROW(::Raindrop::Core::Scene::EntityManager(context, entityCount));
	}

	class EntityManagerFixture : public ContextFixture{
		public:
			EntityManagerFixture() : manager(context, entityCount){}
			::Raindrop::Core::Scene::EntityManager manager;
	};

	TEST_F(EntityManagerFixture, size){
		EXPECT_EQ(manager.size(), entityCount);
	}

	TEST_F(EntityManagerFixture, usedSsize){
		EXPECT_EQ(manager.usedSize(), 0);
	}

	TEST_F(EntityManagerFixture, valid_entityCreation){
		::Raindrop::Core::Scene::EntityID entity;
		EXPECT_NO_THROW(entity = manager.createEntity());
		EXPECT_NE(entity, ::Raindrop::Core::Scene::INVALID_ENTITY_ID);
	}

	class EntityManagerFixtureEntity : public EntityManagerFixture{
		public:
			virtual void SetUp() override{
				EntityManagerFixture::SetUp();
				entity = manager.createEntity();
			}

			::Raindrop::Core::Scene::EntityID entity;
	};

	TEST_F(EntityManagerFixtureEntity, valid_destroy){
		EXPECT_NO_THROW(manager.destroyEntity(entity));
	}

	TEST_F(EntityManagerFixtureEntity, invalid_doubleDestroy){
		EXPECT_NO_THROW(manager.destroyEntity(entity));
		EXPECT_ANY_THROW(manager.destroyEntity(entity));
	}

	class MinimalEntityManagerFixture : public ContextFixture{
		public:
			MinimalEntityManagerFixture() : manager(context, 2){}
			::Raindrop::Core::Scene::EntityManager manager;
	};

	TEST_F(MinimalEntityManagerFixture, valid_entityCreation){
		EXPECT_NO_THROW(manager.createEntity());
		EXPECT_NO_THROW(manager.createEntity());
	}

	TEST_F(MinimalEntityManagerFixture, invalid_entityCreation_outOfSpace){
		EXPECT_NO_THROW(manager.createEntity());
		EXPECT_NO_THROW(manager.createEntity());
		EXPECT_ANY_THROW(manager.createEntity());
	}
}