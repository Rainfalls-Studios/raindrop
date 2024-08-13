#ifndef __RAINDROP_ASSETS_ASSET_HPP__
#define __RAINDROP_ASSETS_ASSET_HPP__

#include <Raindrop/Object.hpp>

namespace Raindrop::Assets{
	class Asset : public Object{
		public:
			virtual ~Asset() = default;
	};
}

#endif