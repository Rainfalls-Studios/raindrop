#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__

#include <common.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::UnitTests::Core{
	class ContextMock : public ::Raindrop::Core::Context{
		public:
			ContextMock() :
				::Raindrop::Core::Context(){}

			MOCK_METHOD(::Raindrop::Core::Registry::Registry&, registry, ());
			MOCK_METHOD(::Raindrop::Core::Registry::Registry&, temp, ());
			MOCK_METHOD(::Raindrop::Core::Event::EventManager&, eventManager, ());
			MOCK_METHOD(::Raindrop::Core::Asset::AssetManager&, assetManager, ());
			MOCK_METHOD(::Raindrop::Core::Scene::SceneManager&, sceneManager, ());
	};

	class ContextFixture : public ::testing::Test {
		public:
			virtual void SetUp() override {
				// ON_CALL(context, assetManager()).WillByDefault(::testing::ReturnRef(assetManagerMock));
				// ON_CALL(contex, registry()).Wil
			}

			ContextMock context;
	};
}

#endif