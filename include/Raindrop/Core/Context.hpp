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
			~Context();

			spdlog::logger& logger();
			
			Registry::Registry& registry();
			Registry::Registry& temp();
			Event::EventManager& eventManager();
			Asset::AssetManager& assetManager();
			Scene::SceneManager& sceneManager();

		private:
			std::shared_ptr<spdlog::logger> _logger;

			std::unique_ptr<Registry::Registry> _registry;
			std::unique_ptr<Registry::Registry> _temp;
			std::unique_ptr<Event::EventManager> _eventManager;
			std::unique_ptr<Asset::AssetManager> _assetManager;
			std::unique_ptr<Scene::SceneManager> _sceneManager;
	};
}

#endif 