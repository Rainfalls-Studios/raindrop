#ifndef __RAINDROP_CORE_SCENE_COMPONENTS_SUN_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENTS_SUN_HPP__

#include <Raindrop/Core/Scene/Component.hpp>
#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Core::Scene::Components{
	class Sun : public Component{
		public:
			Sun();
			virtual ~Sun() override = default;
			
			glm::vec3 color;
			
			virtual void UI(EngineContext& context) override;
	};
}

#endif