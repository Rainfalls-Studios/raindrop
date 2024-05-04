#ifndef __RAINDROP_CORE_ASSETS_MANAGER_HPP__
#define __RAINDROP_CORE_ASSETS_MANAGER_HPP__

#include "common.hpp"
#include "Asset.hpp"

namespace Raindrop::Core::Assets{
	class Manager{
		public:
			Manager(Context &context);
			~Manager();

			/**
			 * @brief 
			 * 
			 * @param type The type of asset to query ("Texture", "Model", ...)
			 * @param path The path of the asset file
			 * @return std::shared_ptr<Asset> 
			 */
			std::shared_ptr<Asset> getAsset(const std::string& type, const Path& path);

			/**
			 * @brief Get the Asset object
			 * 
			 * @tparam T The class type of assets required
			 * @param type The type of asset to query ("Texture", "Model", ...)
			 * @param path The path of the asset file
			 * @return std::shared_ptr<T> 
			 */
			template<typename T>
			inline std::shared_ptr<T> getAsset(const std::string& type, const Path& path);

			/**
			 * @brief Forcefully registers an asset at a given path
			 * 
			 * @param type The type of the asset ("Texture", "Model", ...)
			 * @param path The path to assign for this specific asset
			 * @param asset The asset itself
			 */
			void registerAsset(const std::string& type, const Path& path, const std::shared_ptr<Asset>& asset);
			


			/**
			 * @brief Registers and assigns an asset loader to a given asset type
			 * 
			 * @param type The type of assets the loader handles
			 * @param loader The loader itself
			 */
			void registerLoader(const std::string& type, const std::shared_ptr<Loader>& loader);

			/**
			 * @brief Create and registers a loader
			 * 
			 * @tparam T The type of the loader. It HAS to be derived from the Loader class
			 * @tparam Args 
			 * @param type The type of assets the loader handles
			 * @param args The loader creation arguments
			 */
			template<typename T, typename... Args>
			void createLoader(const std::string& type, Args&&... args);
			
			/**
			 * @brief Find the loader that handles the given asset type
			 * 
			 * @param type The type of assets the handled by the loader
			 * @return std::shared_ptr<Loader> 
			 */
			std::shared_ptr<Loader> findLoader(const std::string& type) const noexcept;

			/**
			 * @brief Unregisters the loader that handles the given asset type
			 * 
			 * @param type The type of assets the handled by the loader
			 */
			void unregisterLoader(const std::string& type);

		private:
			Context& _context;
			
			struct TypeData{
				std::shared_ptr<Loader> _loader;
				std::unordered_map<Path, std::weak_ptr<Asset>> _pathToAssets;
			};

			std::unordered_map<std::string, std::unique_ptr<TypeData>> _typeToLoader;
			TypeData* getTypeData(const std::string& type) const noexcept;
	};
}

#include "Manager.inl"

#endif