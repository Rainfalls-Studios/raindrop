#ifndef __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__
#define __RAINDROP_WRAPPERS_SCENE_WRAPPER_HPP__

#include "common.hpp"
#include "../Core/Scenes/common.hpp"
#include "../Utils/Wrapper.hpp"

namespace Raindrop::Wrappers{
	class SceneWrapper : public Utils::Wrapper<Context, Core::Scenes::Scene, Core::Scenes::SceneID, Core::Scenes::INVALID_SCENE_ID>{
		public:
			using SceneID = Core::Scenes::SceneID;
			using Scene = Core::Scenes::Scene;

			using Wrapper::Wrapper;
			using Wrapper::operator=;
			using Wrapper::operator->;
			using Wrapper::ID;

			virtual ~SceneWrapper() override = default;

			virtual Scene& get() override;
			virtual const Scene& get() const override;
	};
}

#endif