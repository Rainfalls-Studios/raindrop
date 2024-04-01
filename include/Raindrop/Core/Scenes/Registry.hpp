#ifndef __RAINDROP_CORE_SCENES_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENES_REGISTRY_HPP__

#include <Raindrop/Core/Scenes/common.hpp>

namespace Raindrop::Core::Scenes{
	class Registry{
		public:
			Registry(Context& context);
			~Registry();

			SceneID createScene();
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