#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__

#include <common.hpp>
#include <Raindrop/Core/Context.hpp>

class CoreContextFixture : public ::testing::Test{
	public:
		virtual void SetUp() override {
			// ON_CALL(context, assetManager()).WillByDefault(::testing::ReturnRef(assetManagerMock));
			// ON_CALL(contex, registry()).Wil
		}

		Raindrop::Core::Context context;
};

#endif