#ifndef __RAINDROP_CORE_ENGINE_CONTEXT_HPP__
#define __RAINDROP_CORE_ENGINE_CONTEXT_HPP__

#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/TaskQueue.hpp>

namespace Raindrop::Core{
	struct EngineContext{
		Event::EventManager eventManager;
		Asset::AssetManager assetManager;
		Registry::Registry registry;
		
		TaskQueue taskQueue;

		EngineContext(){};
		~EngineContext() = default;
	};
}

#endif