#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"
#include "Core/Scene/Scene.hpp"
#include "Core/Scene/Entity.hpp"
#include "Core/Asset/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"

namespace Raindrop{
	using Scene = Core::Scene::Scene;
	using SceneID = Core::Scene::SceneID;

	using Entity = Core::Scene::Entity;
	using EntityGUID = Core::Scene::EntityGUID;
	using EntityID = EntityGUID;
	using EntityLUID = Core::Scene::EntityLUID;

	using AssetManager = Core::Asset::Manager;
	using EventManager = Core::Event::Manager;

	// Due to name conflicts with the Renderer namespace and the class itself, this does not work
	// TODO: fix class/namespace name conflict
	// using Renderer = Renderer::Renderer;
	
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			Scene& getScene(const SceneID& ID);
			SceneID registerScene(std::unique_ptr<Scene> scene);
			void unregisterScene(const SceneID& ID);

			AssetManager& assetManager();
			EventManager& eventManager();

			Graphics::Renderer& renderer();

			void run();

		private:
			Context* _context;

			void render();
			void events();

			void updateCameraPosition();
	};
}

#endif