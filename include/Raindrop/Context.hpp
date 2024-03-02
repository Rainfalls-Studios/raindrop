#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Asset/Manager.hpp"

namespace Raindrop{
	struct Context{
		Core::Asset::Manager assetManager;
	};
}

#endif