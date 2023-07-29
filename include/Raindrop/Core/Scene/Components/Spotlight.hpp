#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_SPOTLIGHT_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_SPOTLIGHT_HPP__

#include <Raindrop/Core/Scene/Component.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Core::Scene::Components{
	class Spotlight : public Component{
		public:
			Spotlight();
			virtual ~Spotlight() override = default;
			
			glm::vec3 color;
			float maxAngle;
			float intensity;

			virtual void UI(EngineContext& context) override;
	};
}

#endif