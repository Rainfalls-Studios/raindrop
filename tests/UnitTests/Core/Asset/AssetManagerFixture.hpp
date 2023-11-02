#ifndef __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_MANAGER_FIXTURE_HPP__
#define __RAINDROP_TESTS_UNITTESTS_CORE_ASSET_ASSET_MANAGER_FIXTURE_HPP__

#include <Core/Context.hpp>

namespace Raindrop::UnitTests::Core::Asset{
	class AssetManagerFixture : public ContextFixture{
		public:
			AssetManagerFixture() = default;
			~AssetManagerFixture() = default;

			virtual void SetUp() override{
				ContextFixture::SetUp();

				ON_CALL(assetManagerMock, loadOrGet(testing::_)).WillByDefault(
					[this](const std::filesystem::path& path) {
						return this->assetManagerMock.loadOrGet(path);
					}
				);
			}

			virtual void TearDown() override{
				ContextFixture::TearDown();
			}

	};
}

#endif