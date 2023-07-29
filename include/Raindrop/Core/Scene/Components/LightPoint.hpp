#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_LIGHT_POINT_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_LIGHT_POINT_HPP__

#include <Raindrop/Core/Scene/Component.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Core::Scene::Components{
	class LightPoint : public Component{
		public:
			LightPoint();
			virtual ~LightPoint() override = default;
			
			glm::vec3 color;
			float intensity;

			
			virtual void UI(EngineContext& context) override;
	};
}

#endif