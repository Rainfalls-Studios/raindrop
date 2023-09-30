#ifndef __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__
#define __RAINDROP_CORE_ASSET_ASSET_MANAGER_HPP__

#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	class AssetManager{
		public:
			AssetManager();
			~AssetManager();

			void destroyLink(const std::filesystem::path& extension);

			std::weak_ptr<Asset> loadOrGet(const std::filesystem::path& path);

			template<typename T>
			std::weak_ptr<T> loadOrGet(const std::filesystem::path& path){
				return std::static_pointer_cast<T>(loadOrGet(path).lock());
			}

			template<typename T, typename... Args>
			void registerFactory(Args... args){
				registerFactory(std::static_pointer_cast<AssetFactory>(std::make_shared<T>(args...)), typeid(T).hash_code());
			}

			void registerFactory(const std::shared_ptr<AssetFactory>& factory, std::size_t typeID);

			template<typename T>
			void removeFactory(){
				removeFactory(typeid(T).hash_code());
			}

			void removeFactory(std::size_t typeID);

		private:
			std::unordered_map<std::filesystem::path, std::weak_ptr<AssetFactory>> _extensionToFactory;
			std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>> _pathToAsset;

			struct FactoryData{
				std::shared_ptr<AssetFactory> factory;
				std::size_t typeID;
			};

			std::list<FactoryData> _factories;

			AssetFactory* findFactory(const std::filesystem::path& extension);
	};
}

#endif