#ifndef __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Scene/SceneManager.hpp>

namespace Raindrop::Core{
	struct Context{
		spdlog::logger logger;

		Registry::Registry registry;
		Event::EventManager eventManager;
		Asset::AssetManager assetManager;
		Scene::SceneManager sceneManager;

		Context();
		~Context() = default;
	};
}

#endif 