#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_CONTEXT__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_SCENE_CONTEXT__HPP__

#include <common.hpp>
#include <Raindrop/Core/Scene/Context.hpp>
#include <Core/Context.hpp>

class CoreSceneContextFixture : public ::testing::Test{
	public:
		CoreSceneContextFixture() :
			core(),
			context(core)
		{}
		
		testing::NiceMock<::Raindrop::Core::Context> core;
		::Raindrop::Core::Scene::Context context;
};

#endif