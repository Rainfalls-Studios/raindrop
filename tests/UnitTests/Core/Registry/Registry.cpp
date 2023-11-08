#include <common.hpp>
#include <Core/Context.hpp>
#include <Core/Registry/Context.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

TEST_F(CoreContextFixture, valid_registry_constructor){
	EXPECT_NO_THROW(::Raindrop::Core::Registry::Registry(static_cast<::Raindrop::Core::Context&>(context)));
}

class CoreRegistryRegistryFixture : public CoreContextFixture{
	public:
		CoreRegistryRegistryFixture() : registry(context){}
		::Raindrop::Core::Registry::Registry registry;
};

TEST_F(CoreRegistryRegistryFixture, get){
	EXPECT_NO_THROW(registry.get("node"));
	EXPECT_FALSE(registry.empty());
}

TEST_F(CoreRegistryRegistryFixture, remove){
	registry.get("node");
	EXPECT_NO_THROW(registry.remove("node"));
	EXPECT_TRUE(registry.empty());
}

TEST_F(CoreRegistryRegistryFixture, size){
	registry.get("node");
	EXPECT_EQ(registry.size(), 1);
}

TEST_F(CoreRegistryRegistryFixture, same){
	::Raindrop::Core::Registry::Node a = registry.get("node");
	::Raindrop::Core::Registry::Node b = registry.get("node");

	EXPECT_TRUE(a.same(b));
	EXPECT_FALSE(a.different(b));
}

TEST_F(CoreRegistryRegistryFixture, nearDifferent){
	::Raindrop::Core::Registry::Node a = registry.get("node");
	::Raindrop::Core::Registry::Node b = registry.get("mode");

	EXPECT_FALSE(a.same(b));
	EXPECT_TRUE(a.different(b));
}

TEST_F(CoreRegistryRegistryFixture, same_but_different_registry){
	::Raindrop::Core::Registry::Registry secondRegistry(context);

	::Raindrop::Core::Registry::Node a = registry.get("node");
	::Raindrop::Core::Registry::Node b = secondRegistry.get("node");

	EXPECT_FALSE(a.same(b));
	EXPECT_TRUE(a.different(b));
}

TEST_F(CoreRegistryRegistryFixture, set_value){
	registry.get("node").set(5);
	EXPECT_EQ(registry.get("node").as<int>(), 5);
}