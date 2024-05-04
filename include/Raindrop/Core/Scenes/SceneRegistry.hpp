#ifndef __RAINDROP_CORE_SCENES_SCENE_REGISTRY_HPP__
#define __RAINDROP_CORE_SCENES_SCENE_REGISTRY_HPP__

#include <Raindrop/Core/Scenes/common.hpp>
#include <Raindrop/Utils/Registry.hpp>

namespace Raindrop::Core::Scenes{
	class SceneRegistry : private Utils::Registry<std::unique_ptr<Scene>, SceneID>{
		public:
			SceneRegistry(Context& context);
			~SceneRegistry();

			/**
			 * @brief Create a new empty scene
			 * 
			 * @return The scene handle ID
			 */
			SceneID createScene();

			/**
			 * @brief Registrers the given scene into the registry and asigns an ID
			 * 
			 * @param scene the scene to register
			 * @return The scene handle ID
			 */
			SceneID registerScene(std::unique_ptr<Scene>&& scene);

			/**
			 * @brief Removes a scene from the registry
			 * 
			 * @param ID The scene's ID
			 */
			void unregisterScene(const SceneID& ID);

			/**
			 * @brief Gives a scene from it's ID
			 * 
			 * @param ID The scene's ID
			 * @return A reference to the scene
			 * 
			 * 
			 * @throws std::runtime_error on invalid ID
			 */
			Scene& get(const SceneID& ID);
			
			/**
			 * @brief Checks if a scene ID is valid
			 * 
			 * @param ID The ID to check
			 * @return true if the ID is asigned to a scene, false otherwise
			 */
			using Registry::exists;

			/**
			 * @brief Checks if a scene ID is in the range of the registry
			 * 
			 * @param ID The ID to check
			 * @return true if the ID is in range, false otherwisze
			 */
			using Registry::inRange;

			/**
			 * @brief Gives the reserved size of the registry
			 * 
			 * @return The max registrable handles before reallocation
			 */
			using Registry::reservedSize;

			/**
			 * @brief The unsused handles count
			 * 
			 */
			using Registry::freeSize;

			/**
			 * @brief The total number of registred scene into the registry
			 * 
			 */
			using Registry::usedSize;

		private:
			Context& _context;
	};
}

#endif