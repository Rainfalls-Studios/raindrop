#ifndef __RAINDROP_SCENE_HPP__
#define __RAINDROP_SCENE_HPP__

#include <Raindrop/common.hpp>
#include <Raindrop/Entity.hpp>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop{
	class Scene{
		public:
			Scene(const std::shared_ptr<Core::Scene::Scene>& scene);
			Scene(const Scene& other);
			Scene();

			~Scene();

			Scene& operator=(const std::shared_ptr<Core::Scene::Scene>& scene);
			Scene& operator=(const Scene& other);
			
			bool operator==(const std::shared_ptr<Core::Scene::Scene>& scene) const;
			bool operator==(const Scene& other) const;
			bool operator!=(const std::shared_ptr<Core::Scene::Scene>& scene) const;
			bool operator!=(const Scene& other) const;

			Entity root();
			const Entity root() const;

			Entity createEntity();
			Entity createEntity(Entity parent);
			void destroyEntity(Entity entity);

			template<typename T> const std::list<Entity>& filterEntitiesWithComponent() const;

		private:
			std::shared_ptr<Core::Scene::Scene> _scene;

	};

	// #include <Raindrop/tpp/Scene.tpp>
}

#endif