#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_MANAGER_MOCK__HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_MANAGER_MOCK__HPP__

#include <common.hpp>
#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Asset{
	class AssetManagerMock : public ::Raindrop::Core::Asset::AssetManager{
		public:
			AssetManagerMock(::Raindrop::Core::Context& context) : AssetManager(context){}

			MOCK_METHOD(std::weak_ptr<::Raindrop::Core::Asset::Asset>, loadOrGet, (const std::filesystem::path& path));
			MOCK_METHOD(void, registerFactory, (const std::shared_ptr<::Raindrop::Core::Asset::AssetFactory>& factory, std::size_t typeID));
			MOCK_METHOD(void, removeFactory, (std::size_t typeID));
	};
}

#endif