#ifndef __RAINDROP_ASSET_HPP__
#define __RAINDROP_ASSET_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Core/Asset/Asset.hpp>

namespace Raindrop{
	// TODO: refactor with a custom class
	using Asset = std::weak_ptr<Core::Asset::Asset>;
}

#endif