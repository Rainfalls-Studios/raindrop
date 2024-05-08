#ifndef __RAINDROP_ASSET_HPP__
#define __RAINDROP_ASSET_HPP__

#include "common.hpp"

namespace Raindrop{
	class Asset{
		public:
			class Loader{
				public:
					virtual ~Loader() = default;

					/**
					 * @brief Supposedly destroys unused assets
					 * 
					 */
					virtual void collectGarbage();
					
					/**
					 * @brief Load an asset from a given path
					 * 
					 * @param path The path to the asset relative or absolut
					 * @return std::shared_ptr<Asset> 
					 */
					virtual std::shared_ptr<Asset> load(const Path& path) = 0;
			};

			Asset() = default;
			virtual ~Asset() = default;


			/**
			 * @brief Creates an asset loader and registers it
			 * 
			 * @tparam T The loader type
			 * @tparam Args The loader constructor arguments types
			 * @param context The engine context
			 * @param assetType The asset type to which the loader is linked
			 * @param args The loader constructor arguments
			 * @return std::shared_ptr<T> 
			 */
			template<typename T, typename... Args>
			static std::shared_ptr<T> CreateLoader(Context& context, const std::string& assetType, Args&&... args);

			/**
			 * @brief Registers an already created loader
			 * 
			 * @tparam T The loader type
			 * @param context The engine context
			 * @param assetType The asset type to which the loader is linked
			 * @param loader The loader itself
			 */
			template<typename T>
			static void RegisterLoader(Context& context, const std::string& assetType, const std::shared_ptr<T>& loader);

			/**
			 * @brief Unregiters a loader from it's asset type
			 * 
			 * @param context The engine context
			 * @param assetType The asset type to which the loader is linked
			 */
			static void UnregisterLoader(Context& context, const std::string& assetType);


			/**
			 * @brief Loads or gets an asset from the given path if it can be found
			 * 
			 * @tparam T The asset type to load
			 * @param context The engine context
			 * @param assetType The asset type. Specified to know which loader to used
			 * @param path The path to the asset file 
			 * @return T The asset
			 */
			template<typename T>
			static std::shared_ptr<T> Load(Context& context, const std::string& assetType, const Path& path);

		private:
			static void _RegisterLoader(Context& context, const std::string& assetType, const std::shared_ptr<Loader>& loader);
			static std::shared_ptr<Asset> _Load(Context& context, const std::string& assetType, const Path& path);

	};
}

#include "Inline/Asset.inl"

#endif