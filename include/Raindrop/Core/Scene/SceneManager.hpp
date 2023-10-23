#ifndef __RAINDROP_CORE_SCENE_SCENE_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SCENE_MANAGER_HPP__

#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	class SceneManager{
		public:
			SceneManager(Core::Context& core);
			~SceneManager();

			std::shared_ptr<Scene> create(const std::string& name);
			std::shared_ptr<Scene> getOrCreate(const std::string& name);
			std::shared_ptr<Scene> get(const std::string& name);

			const std::shared_ptr<Scene> get(const std::string& name) const;

			void remove(const std::string& name);
			void remove(const std::shared_ptr<Scene>& scene);
			bool exists(const std::string& name) const;

		private:
			std::unique_ptr<Context> _context;

			std::unordered_map<std::string, std::shared_ptr<Scene>> _nameToSceneMap;
	};
}

#endif