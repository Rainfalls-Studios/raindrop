#ifndef __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>

namespace Raindrop::Core{
    struct Context{
        spdlog::logger logger;

		Event::EventManager eventManager;
		Asset::AssetManager assetManager;
		Registry::Registry registry;

		Context();
		~Context() = default;
    };
}

#endif 