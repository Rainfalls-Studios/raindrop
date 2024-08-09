#ifndef __RAINDROP_ASSETS_FACTORY_HPP__
#define __RAINDROP_ASSETS_FACTORY_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Assets{
	class Factory{
		public:
			virtual ~Factory() = default;
			
			virtual std::shared_ptr<Asset> getOrCreate(const std::filesystem::path& path) = 0;
	};
}

#endif