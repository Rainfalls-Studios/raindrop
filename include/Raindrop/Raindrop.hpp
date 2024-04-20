#ifndef __RAINDROP_RAINDROP_HPP__
#define __RAINDROP_RAINDROP_HPP__

#include "common.hpp"
#include "Core/Scenes/Scene.hpp"
#include "Core/Scenes/Property.hpp"
#include "Core/Scenes/Entity.hpp"
#include "Core/Assets/Manager.hpp"
#include "Core/Event/Manager.hpp"
#include "Graphics/Renderer.hpp"
#include "Modules/Module.hpp"
#include "Camera.hpp"

#include "Wrappers/SceneWrapper.hpp"

namespace Raindrop{
	using Scene = Core::Scenes::Scene;
	using SceneID = Core::Scenes::SceneID;
	using SceneProperty = Core::Scenes::Property;

	using Entity = Core::Scenes::Entity;
	using EntityGUID = Core::Scenes::EntityGUID;
	using EntityID = EntityGUID;
	using EntityLUID = Core::Scenes::EntityLUID;

	using Asset = Core::Assets::Asset;
	using AssetLoader = Core::Assets::Loader;
	using Module = Modules::Module;

	using AssetManager = Core::Assets::Manager;
	using EventManager = Core::Event::Manager;
	using Renderer = Graphics::Renderer;

	using Key = Core::Event::Key;
	using KeyMod = Core::Event::KeyMod;

	using namespace Wrappers;
	
	class Raindrop{
		public:
			Raindrop();
			~Raindrop();

			// === Scenes ===

			SceneWrapper createScene();
			SceneWrapper registerScene(std::unique_ptr<Scene> scene);
			void unregisterScene(const SceneWrapper& ID);

			SceneWrapper getScene(const SceneID& ID);

			// === Events ===

			EventManager& eventManager();

			template<typename F>
			inline constexpr void subscribeEvent(const std::string& name, F&& callback);

			template<typename... Args>
			inline constexpr void triggerEvent(const std::string& name, Args... args);

			template<typename... Args>
			inline constexpr void registerEvent(const std::string& name);

			// === Assets ===

			AssetManager& assetManager();

			std::shared_ptr<Asset> getAsset(const std::string& type, const Path& path);
			void registerAssetLoader(const std::string& type, const std::shared_ptr<AssetLoader>& loader);
			void unregisterAssetLoader(const std::string& type);
			std::shared_ptr<AssetLoader> getAssetLoader(const std::string& type);

			template<typename T>
			inline constexpr std::shared_ptr<T> getAsset(const std::string& type, const Path& path);

			template<typename T, typename... Args>
			inline constexpr void createAssetLoader(const std::string& type, Args&&... args);

			template<typename T>
			inline constexpr std::shared_ptr<T> getAssetLoader(const std::string& type);

			// === Modules ===
			std::shared_ptr<Module> getModule(const std::string& alias);
			std::shared_ptr<Module> loadModule(const Path& path);

			// === Graphics ===
			
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

#include "Raindrop.inl"

#endif