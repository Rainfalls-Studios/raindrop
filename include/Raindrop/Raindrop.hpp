#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"
#include "Core/Scenes/Scene.hpp"
#include "Core/Scenes/Entity.hpp"
#include "Core/Assets/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"

namespace Raindrop{
	using Scene = Core::Scenes::Scene;
	using SceneID = Core::Scenes::SceneID;

	using Entity = Core::Scenes::Entity;
	using EntityGUID = Core::Scenes::EntityGUID;
	using EntityID = EntityGUID;
	using EntityLUID = Core::Scenes::EntityLUID;

	using AssetManager = Core::Assets::Manager;
	using EventManager = Core::Event::Manager;

	using Renderer = Graphics::Renderer;
	
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			// === Scene ===

			Scene& getScene(const SceneID& ID);
			SceneID registerScene(std::unique_ptr<Scene> scene);
			void unregisterScene(const SceneID& ID);

			// === Events ===

			EventManager& eventManager();

			template<typename F>
			inline constexpr void subscribeEvent(const std::string& name, F&& callback){
				eventManager().subscribe(name, callback);
			}

			template<typename... Args>
			inline constexpr void triggerEvent(const std::string& name, Args... args){
				eventManager().trigger<Args...>(name, args...);
			}

			template<typename... Args>
			inline constexpr void registerEvent(const std::string& name){
				eventManager().registerEvent<Args...>(name);
			}

			// === Assets ===

			AssetManager& assetManager();

			

			Renderer& renderer();

			// === Runtime ===

			void run();
			void quit();

		private:
			Context* _context;

			void render();
			void events();

			void updateCameraPosition();
	};
}

#endif