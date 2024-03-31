#ifndef __RAINDROP_CORE_SCENE_SCENES_HPP__
#define __RAINDROP_CORE_SCENE_SCENES_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Scenes{
		public:
			Scenes(Context& context);
			~Scenes();

			template<typename T, typename... Args>
			SceneID createScene(Args&&... args){
				static_assert(std::is_base_of_v<Scene, T>, "Cannot create a scene from a class that is not derived from Raindrop::Scene (Raindrop::Core::Scene::Scene)");
				std::unique_ptr<T> scene = std::make_unique<T>(args...);
				return registerScene(std::move(scene));
			}

			SceneID registerScene(std::unique_ptr<Scene> scene);
			void unregisterScene(const SceneID& ID);

			Scene& get(const SceneID& ID);

		private:
			Context& _context;

			std::vector<std::unique_ptr<Scene>> _scenes;
			std::queue<SceneID> _freeIDs;
	};
}

#endif