#ifndef __RAINDROP_GRAPHICS_COMPONENTS_SPOTLIGHT_HPP__
#define __RAINDROP_GRAPHICS_COMPONENTS_SPOTLIGHT_HPP__

#include <Raindrop/Graphics/Components/common.hpp>

namespace Raindrop::Graphics::Components{
	class Spotlight : public Component{
		public:
			Spotlight();
			virtual ~Spotlight() override = default;
			
			glm::vec3 color;
			float outerCutOff;
			float cutOff;
			float intensity;
	};
}

#endif