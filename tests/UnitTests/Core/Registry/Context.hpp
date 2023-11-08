#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_CONTEXT__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_CONTEXT__HPP__

#include <common.hpp>
#include <Raindrop/Core/Registry/Context.hpp>
#include <Core/Context.hpp>

class CoreRegistryContextFixture : public ::testing::Test{
	public:
		CoreRegistryContextFixture() :
			core(),
			context(core)
		{}
		
		testing::NiceMock<::Raindrop::Core::Context> core;
		::Raindrop::Core::Registry::Context context;
};

#endif