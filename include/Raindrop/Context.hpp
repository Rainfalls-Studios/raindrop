#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Asset/Manager.hpp"
#include "Core/Event/Manager.hpp"

namespace Raindrop{
	struct Context{
		Core::Asset::Manager assetManager;
		Core::Event::Manager eventManager;
	};
}

#endif