#ifndef __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	// TODO: add asset root path
	class AssetManager{
		friend class Core::Engine;
		public:
			AssetManager(Core::Context& core);
			~AssetManager();

			std::shared_ptr<Asset> loadOrGet(const std::filesystem::path& path);

			template<typename T> std::shared_ptr<T> loadOrGet(const std::filesystem::path& path);
			template<typename T, typename... Args> std::shared_ptr<T> registerFactory(Args... args);

			void registerFactory(const std::shared_ptr<AssetFactory>& factory);
			void removeFactory(const std::shared_ptr<AssetFactory>& factory);

		private:
			std::unique_ptr<Context> _context;

			std::unordered_map<std::filesystem::path, std::weak_ptr<AssetFactory>> _extensionToFactory;
			std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>> _pathToAsset;

			AssetFactory* findFactory(const std::filesystem::path& extension);
	};

	#include <Raindrop/Core/Asset/tpp/AssetManager.tpp>
}

#endif