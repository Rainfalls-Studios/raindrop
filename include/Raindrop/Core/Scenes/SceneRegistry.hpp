#ifndef __RAINDROP_CORE_SCENES_SCENE_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENES_SCENE_REGISTRY_HPP__

#include <Raindrop/Core/Scenes/common.hpp>
#include <Raindrop/Utils/Registry.hpp>

namespace Raindrop::Core::Scenes{
	class SceneRegistry : private Utils::Registry<std::unique_ptr<Scene>, SceneID>{
		public:
			SceneRegistry(Context& context);
			~SceneRegistry();

			SceneID createScene();
			SceneID registerScene(std::unique_ptr<Scene>&& scene);
			void unregisterScene(const SceneID& ID);

			Scene& get(const SceneID& ID);

			using Registry::exists;
			using Registry::inRange;

			using Registry::reservedSize;
			using Registry::freeSize;
			using Registry::usedSize;

		private:
			Context& _context;
	};
}

#endif