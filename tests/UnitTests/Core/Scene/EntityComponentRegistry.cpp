#include <common.hpp>
#include <Core/Scene/Context.hpp>
#include <Raindrop/Core/Scene/EntityComponentsRegistry.hpp>

static constexpr std::size_t entityCount = 150;
static constexpr std::size_t componentCount = 1000;

namespace Raindrop::UnitTests::Core::Scene{
	TEST_F(ContextFixture, valid_EntityComponentRegistryFixture_constructor){
		EXPECT_NO_THROW(::Raindrop::Core::Scene::EntityComponentsRegistry(context, entityCount, componentCount));
	}

	class EntityComponentRegistryFixture : public ContextFixture{
		public:
			EntityComponentRegistryFixture() : registry{context, entityCount, componentCount}{}

			virtual void SetUp() override{
				ContextFixture::SetUp();
			}

			::Raindrop::Core::Scene::EntityComponentsRegistry registry;
	};

	// METHODS
	TEST_F(EntityComponentRegistryFixture, valid_get){
		EXPECT_NO_THROW(registry.get(0, 0));
	}

	TEST_F(EntityComponentRegistryFixture, invalid_get_entityOutOfRance){
		EXPECT_THROW(registry.get(entityCount * 2, 0), std::out_of_range);
	}

	TEST_F(EntityComponentRegistryFixture, invalid_get_componentOutOfRange){
		EXPECT_THROW(registry.get(0, componentCount * 2), std::out_of_range);
	}

	TEST_F(EntityComponentRegistryFixture, valid_set){
		EXPECT_NO_THROW(registry.set(0, 0, 0));
	}

	TEST_F(EntityComponentRegistryFixture, invalid_set_entityOutOfRance){
		EXPECT_THROW(registry.set(entityCount * 2, 0, 0), std::out_of_range);
	}

	TEST_F(EntityComponentRegistryFixture, invalid_set_componentOutOfRance){
		EXPECT_THROW(registry.set(0, componentCount * 2, 0), std::out_of_range);
	}

	TEST_F(EntityComponentRegistryFixture, entityCount){
		EXPECT_EQ(registry.entityCount(), entityCount);
	}

	TEST_F(EntityComponentRegistryFixture, componentCount){
		EXPECT_EQ(registry.componentCount(), componentCount);
	}
}