#ifndef __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	class AssetManager{
		public:
			AssetManager(Core::Context& core);
			~AssetManager();

			std::weak_ptr<Asset> loadOrGet(const std::filesystem::path& path);

			template<typename T> std::weak_ptr<T> loadOrGet(const std::filesystem::path& path);
			template<typename T, typename... Args> void registerFactory(Args... args);
			template<typename T> void removeFactory();

			void registerFactory(const std::shared_ptr<AssetFactory>& factory, std::size_t typeID);
			void removeFactory(std::size_t typeID);

		private:
			struct FactoryData;

			std::unique_ptr<Context> _context;

			std::unordered_map<std::filesystem::path, std::weak_ptr<AssetFactory>> _extensionToFactory;
			std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>> _pathToAsset;
			std::list<FactoryData> _factories;

			AssetFactory* findFactory(const std::filesystem::path& extension);
	};

	#include <Raindrop/Core/Asset/tpp/AssetManager.tpp>
}

#endif