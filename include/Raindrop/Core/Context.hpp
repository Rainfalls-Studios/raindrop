#ifndef __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_CORE_CONTEXT_HPP__

#include <spdlog/spdlog.h>

#include <Raindrop/Core/Asset/AssetManager.hpp>
#include <Raindrop/Core/Event/EventManager.hpp>
#include <Raindrop/Core/Registry/Registry.hpp>
#include <Raindrop/Core/Scene/SceneManager.hpp>

namespace Raindrop::Core{
	class Context{
		public:
			Context();
			~Context() = default;

			spdlog::logger& logger();
			
			Registry::Registry& registry();
			Registry::Registry& temp();
			Event::EventManager& eventManager();
			Asset::AssetManager& assetManager();
			Scene::SceneManager& sceneManager();

		private:
			spdlog::logger _logger;

			Registry::Registry _registry;
			Registry::Registry _temp;
			Event::EventManager _eventManager;
			Asset::AssetManager _assetManager;
			Scene::SceneManager _sceneManager;
	};
}

#endif 