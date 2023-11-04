#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_CONTEXT__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_REGISTRY_CONTEXT__HPP__

#include <common.hpp>
#include <Raindrop/Core/Registry/Context.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Registry{
	class ContextMock : public ::Raindrop::Core::Registry::Context{
		public:
			ContextMock(::Raindrop::Core::Context& context) : Context(context){};

			MOCK_METHOD(::Raindrop::Core::Registry::Context&, core, ());
	};

	class ContextFixture : public ::testing::Test{
		public:
			ContextFixture() :
				core(),
				context(core)
			{}

			virtual void SetUp() override{
				ON_CALL(context, core()).WillByDefault(::testing::ReturnRef(core));
			}
			
			testing::NiceMock<Core::ContextMock> core;
			ContextMock context;
	};
}

#endif