#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "common.hpp"
#include "Core/Asset/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Renderer/Renderer.hpp"

namespace Raindrop{
	struct Context{
		Context();
		~Context() = default;

		Core::Asset::Manager assetManager;
		Core::Event::Manager eventManager;
		Renderer::Renderer renderer;
	};
}

#endif