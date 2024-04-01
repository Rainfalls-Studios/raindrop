#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"
#include "Core/Scenes/Scene.hpp"
#include "Core/Scenes/Entity.hpp"
#include "Core/Assets/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"

// TODO: Add Raindrop.inl

namespace Raindrop{
	using Scene = Core::Scenes::Scene;
	using SceneID = Core::Scenes::SceneID;

	using Entity = Core::Scenes::Entity;
	using EntityGUID = Core::Scenes::EntityGUID;
	using EntityID = EntityGUID;
	using EntityLUID = Core::Scenes::EntityLUID;

	using Asset = Core::Assets::Asset;
	using AssetLoader = Core::Assets::Loader;


	using AssetManager = Core::Assets::Manager;
	using EventManager = Core::Event::Manager;
	using Renderer = Graphics::Renderer;
	
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			// === Scenes ===

			SceneID createScene();
			SceneID registerScene(std::unique_ptr<Scene> scene);
			void unregisterScene(const SceneID& ID);

			Scene& getScene(const SceneID& ID);

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

			std::shared_ptr<Asset> getAsset(const std::string& type, const Path& path);

			template<typename T>
			inline constexpr std::shared_ptr<T> getAsset(const std::string& type, const Path& path){
				static_assert(std::is_base_of_v<Asset, T> && "The asset type as to be based of Raindrop::Asset (Raindrop::Core::Assets::Asset)");
				return std::static_pointer_cast<T>(getAsset(type, path));
			}

			void registerAssetLoader(const std::string& type, const std::shared_ptr<AssetLoader>& loader);

			template<typename T, typename... Args>
			inline constexpr void createAssetLoader(const std::string& type, Args&&... args){
				static_assert(std::is_base_of_v<AssetLoader, T> && "The asset loader has to be based of Raindrop::AssetLoader (Raindrop::Core::Assets::Loader)");
				registerAssetLoader(type, std::make_shared<T>(args...));
			}

			void unregisterAssetLoader(const std::string& type);

			std::shared_ptr<AssetLoader> getAssetLoader(const std::string& type);

			template<typename T>
			inline constexpr std::shared_ptr<T> getAssetLoader(const std::string& type){
				return std::static_pointer_cast<T>(getAssetLoader(type));
			}
			
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