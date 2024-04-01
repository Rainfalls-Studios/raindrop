#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include "common.hpp"
#include "../Core/Scenes/common.hpp"

namespace Raindrop::Wrappers{
	class SceneWrapper{
		public:
			using SceneID = Core::Scenes::SceneID;
			using Scene = Core::Scenes::Scene;

			SceneWrapper();
			SceneWrapper(Context& context, const SceneID& ID);
			SceneWrapper(const SceneWrapper& other);

			SceneWrapper& operator=(const SceneWrapper& other);
			SceneWrapper& operator=(const SceneID& ID);

			~SceneWrapper();

			Scene& get();
			const Scene& get() const;

			SceneID ID() const;

			Scene* operator->();
			const Scene* operator->() const;

		private:
			Context* _context;
			SceneID _ID;

	};
}

#endif