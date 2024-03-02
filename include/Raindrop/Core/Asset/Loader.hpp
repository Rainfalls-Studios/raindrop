#ifndef __RAINDROP_CORE_ASSET_LOADER_HPP__
#define __RAINDROP_CORE_ASSET_LOADER_HPP__

#include "common.hpp"

namespace Raindrop::Core::Asset{
	class Loader{
		public:
			virtual ~Loader() = default;

			virtual std::shared_ptr<Asset> load(const Path& path) = 0;
	};
}

#endif