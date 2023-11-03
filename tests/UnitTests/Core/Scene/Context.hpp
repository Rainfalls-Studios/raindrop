#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_CONTEXT__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_CONTEXT__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/Context.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Scene{
	class ContextMock : public ::Raindrop::Core::Scene::Context{
		public:
			ContextMock(::Raindrop::Core::Context& context) : Context(context){};

			MOCK_METHOD(::Raindrop::Core::Context&, core, ());
	};

	class ContextFixture : public ::testing::Test{
		public:
			ContextFixture() :
				core(),
				context(core)
			{}

			virtual void SetUp() override{
				ON_CALL(context, core).WillByDefault(::testing::ReturnRef(core));
			}
			
			testing::NiceMock<Core::ContextMock> core;
			ContextMock context;
	};
}

#endif