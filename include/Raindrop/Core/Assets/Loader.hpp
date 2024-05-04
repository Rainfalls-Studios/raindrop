#ifndef __RAINDROP_CORE_ASSETS_LOADER_HPP__
#define __RAINDROP_CORE_ASSETS_LOADER_HPP__

#include "common.hpp"

namespace Raindrop::Core::Assets{
	class Loader{
		public:
			virtual ~Loader() = default;

			/**
			 * @brief Supposedly destroys unused assets
			 * 
			 */
			virtual void collectGarbage() = 0;

			/**
			 * @brief Load an asset from a given path
			 * 
			 * @param path The path to the asset relative or absolut
			 * @return std::shared_ptr<Asset> 
			 */
			virtual std::shared_ptr<Asset> load(const Path& path) = 0;

			/**
			 * @brief Forcefully registers an asset into the loader
			 * 
			 * @param path The path to the asset
			 * @param asset The asset itself
			 */
			virtual void registerAsset(const Path& path, const std::shared_ptr<Asset>& asset) = 0;
	};
}

#endif