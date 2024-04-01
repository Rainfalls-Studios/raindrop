#ifndef __RAINDROP_CORE_ASSETS_LOADER_HPP__
#define __RAINDROP_CORE_ASSETS_LOADER_HPP__

#include "common.hpp"

namespace Raindrop::Core::Assets{
	class Loader{
		public:
			virtual ~Loader() = default;

			virtual void collectGarbage() = 0;
			virtual std::shared_ptr<Asset> load(const Path& path) = 0;

			virtual void registerAsset(const Path& path, const std::shared_ptr<Asset>& asset) = 0;
	};
}

#endif