#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/Entity.hpp"
#include "Core/Asset/Manager.hpp"
#include "Core/Event/Manager.hpp"

namespace Raindrop{
	using Scene = Core::Scene::Scene;
	using Entity = Core::Scene::Entity;
	using AssetManager = Core::Asset::Manager;
	using EventManager = Core::Event::Manager;
	
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			Scene& scene();
			AssetManager& assetManager();
			EventManager& eventManager();

			void run();

		private:
			Context* _context;

			void render();
			void events();

			void updateCameraPosition();
	};
}

#endif