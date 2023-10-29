#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_CONTEXT_HPP__

#include <common.hpp>
#include <Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::UnitTests::Core{
	class ContextMock : public ::Raindrop::Core::Context{
		public:
			ContextMock() :
				::Raindrop::Core::Context(){}

			MOCK_METHOD(void, Context, ());

			MOCK_METHOD(spdlog::logger&, logger, ());
			MOCK_METHOD(::Raindrop::Core::Registry::Registry&, registry, ());
			MOCK_METHOD(::Raindrop::Core::Registry::Registry&, temp, ());
			MOCK_METHOD(::Raindrop::Core::Event::EventManager&, eventManager, ());
			MOCK_METHOD(::Raindrop::Core::Asset::AssetManager&, assetManager, ());
			MOCK_METHOD(::Raindrop::Core::Scene::SceneManager&, sceneManager, ());

	};

	class ContextFixture : public ::testing::Test {
		public:
			ContextFixture() : 
				assetManagerMock(context){}

			virtual void SetUp() override {
				ON_CALL(context, logger()).WillByDefault(::testing::Return(context.logger()));
				ON_CALL(context, assetManager()).WillByDefault(::testing::Return(::testing::Ref(assetManagerMock)));
			}

			ContextMock context;
			Asset::AssetManagerMock assetManagerMock;
	};
}

#endif