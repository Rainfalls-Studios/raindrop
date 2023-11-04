#include <common.hpp>
#include <Core/Context.hpp>
#include <Core/Registry/Context.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

// This tests both the registry and the nodes since the registry is a node

namespace Raindrop::UnitTests::Core{
	TEST_F(ContextFixture, valid_registry_constructor){
		EXPECT_NO_THROW(::Raindrop::Core::Registry::Registry(static_cast<::Raindrop::Core::Context&>(context)));
	}

	namespace Registry{
		class RegistryFixture : public ContextFixture{
			public:
				RegistryFixture() : registry(core){}
				::Raindrop::Core::Registry::Registry registry;
		};

		TEST_F(RegistryFixture, get){
			EXPECT_NO_THROW(registry.get("node"));
			EXPECT_FALSE(registry.empty());
		}

		TEST_F(RegistryFixture, remove){
			registry.get("node");
			EXPECT_NO_THROW(registry.remove("node"));
			EXPECT_TRUE(registry.empty());
		}

		TEST_F(RegistryFixture, size){
			registry.get("node");
			EXPECT_EQ(registry.size(), 1);
		}

		TEST_F(RegistryFixture, same){
			::Raindrop::Core::Registry::Node a = registry.get("node");
			::Raindrop::Core::Registry::Node b = registry.get("node");

			EXPECT_TRUE(a.same(b));
			EXPECT_FALSE(a.different(b));
		}

		TEST_F(RegistryFixture, nearDifferent){
			::Raindrop::Core::Registry::Node a = registry.get("node");
			::Raindrop::Core::Registry::Node b = registry.get("mode");

			EXPECT_FALSE(a.same(b));
			EXPECT_TRUE(a.different(b));
		}

		TEST_F(RegistryFixture, same_but_different_registry){
			::Raindrop::Core::Registry::Registry secondRegistry(core);

			::Raindrop::Core::Registry::Node a = registry.get("node");
			::Raindrop::Core::Registry::Node b = secondRegistry.get("node");

			EXPECT_FALSE(a.same(b));
			EXPECT_TRUE(a.different(b));
		}

		TEST_F(RegistryFixture, set_value){
			registry.get("node").as<int>() = 5;
			EXPECT_EQ(registry.get("node").as<int>(), 5);
		}
	}
}