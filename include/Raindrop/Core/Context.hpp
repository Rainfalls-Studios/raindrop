#ifndef __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/TaskQueue.hpp>
#include <Raindrop/Core/Configuration.hpp>

namespace Raindrop::Core{
    struct Context{
		Event::EventManager eventManager;
		Asset::AssetManager assetManager;
		Registry::Registry registry;

		std::filesystem::path rootDirectory;
		std::filesystem::path dataDirectory;
		Configuration config;
		
		TaskQueue taskQueue;
        spdlog::logger logger;

		Context();
		~Context() = default;
    };
}

#endif 