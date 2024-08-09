#ifndef __RAINDROP_CONTEXT_HPP__
#define __RAINDROP_CONTEXT_HPP__

#include "types.hpp"
#include "pch.pch"
#include "UUIDRegistry.hpp"
#include "Assets/Context.hpp"
#include "Graphics/Context.hpp"
#include "Events/Context.hpp"
#include "Scenes/Context.hpp"

namespace Raindrop{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		UUIDRegistry registry;
		std::unique_ptr<Assets::Context> assets;
		std::unique_ptr<Events::Context> events;
		std::unique_ptr<Graphics::Context> graphics;
		std::unique_ptr<Scenes::Context> scenes;

		void createLogger();
		void initialize();

		void initializeRegistry();
		void initializeAssets();
		void initializeEvents();
		void initializeGraphics();
		void initializeScenes();
	};
}

#endif