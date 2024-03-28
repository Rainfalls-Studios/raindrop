#ifndef __RAINDROP_CORE_SCENE_SCENES_HPP__
#define __RAINDROP_CORE_SCENE_SCENES_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class Scenes{
		public:
			Scenes(Context& context);
			~Scenes();

			SceneID registerScene(const std::shared_ptr<Scene>& scene);
			void unregisterScene(const SceneID& ID);

			Scene& get(const SceneID& ID);

		private:
			Context& _context;

			std::vector<std::shared_ptr<Scene>> _scenes;
			std::queue<SceneID> _freeIDs;
	};
}

#endif