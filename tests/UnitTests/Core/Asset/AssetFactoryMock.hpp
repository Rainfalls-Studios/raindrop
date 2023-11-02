#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_FACTORY_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_FACTORY_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Asset/AssetFactory.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Asset{
	class AssetFactoryMock : public ::Raindrop::Core::Asset::AssetFactory{
		public:
			AssetFactoryMock(::Raindrop::Core::Asset::Context& context) : AssetFactory(context){}

			MOCK_METHOD(std::shared_ptr<::Raindrop::Core::Asset::Asset>, createAsset, (const std::filesystem::path& path), (override));
			MOCK_METHOD(void, destroyAsset, (std::shared_ptr<::Raindrop::Core::Asset::Asset>), (override));
			MOCK_METHOD(std::vector<const char*>, extensions, ());
	};
}

#endif