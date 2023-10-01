#ifndef __RAINDROP_GRAPHICS_COMPONENTS_LIGHT_POINT_HPP__
#define __RAINDROP_GRAPHICS_COMPONENTS_LIGHT_POINT_HPP__

#include <Raindrop/Graphics/Components/common.hpp>

namespace Raindrop::Graphics::Components{
	class LightPoint : public Component{
		public:
			LightPoint();
			virtual ~LightPoint() override = default;
			
			glm::vec3 color;
			float intensity;
	};
}

#endif