#ifndef __RAINDROP_CORE_ENGINE_HPP__
#define __RAINDROP_CORE_ENGINE_HPP__

#include <Raindrop/Core/common.hpp>
#include <Raindrop/Core/Event/common.hpp>
#include <Raindrop/Core/Asset/common.hpp>
#include <Raindrop/Core/Registry/common.hpp>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core{
	class Engine{
		public:
			Engine();
			~Engine();

			Event::EventManager& eventManager();
			const Event::EventManager& eventManager() const;

			void triggerEvent(const std::string& event);
			void subscribeEvent(const std::string& event, Event::Callback callback);
			
			Event::KeyEvents& keyEvents();
			Event::MouseEvents& mouseEvents();
			const Event::KeyEvents& keyEvents() const;
			const Event::MouseEvents& mouseEvents() const;

			Asset::AssetManager& assetManager();
			const Asset::AssetManager& assetManager() const;

			std::shared_ptr<Asset::Asset> loadOrGet(const std::filesystem::path& path);
			void registerFactory(const std::shared_ptr<Asset::AssetFactory>& factory);
			void removeFactory(const std::shared_ptr<Asset::AssetFactory>& factory);

			template<typename T> std::shared_ptr<T> loadOrGet(const std::filesystem::path& path);
			template<typename T, typename... Args> void registerFactory(Args... args);

			Scene::SceneManager& sceneManager();
			const Scene::SceneManager& sceneManager() const;

			std::shared_ptr<Scene::Scene> createScene(const std::string& name);
			std::shared_ptr<Scene::Scene> getOrCreateScene(const std::string& name);
			std::shared_ptr<Scene::Scene> getScene(const std::string& name);
			const std::shared_ptr<Scene::Scene> getScene(const std::string& name) const;
			
			void removeScene(const std::string& name);
			void removeScene(const std::shared_ptr<Scene::Scene>& scene);
			bool existsScene(const std::string& name) const;

			Registry::Registry& registry();
			const Registry::Registry& registry() const;

			Context& context();

		private:
			std::unique_ptr<Context> _context;

			Asset::Context& assetContext();
	};
	#include <Raindrop/Core/tpp/Engine.tpp>
}

#endif