#ifndef __RAINDROP_GRAPHICS_BASE_RENDER_BASE_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_BASE_RENDER_BASE_RENDERER_HPP__

#include <Raindrop/Graphics/BaseRender/common.hpp>

namespace Raindrop::Graphics::BaseRender{
	class BaseRenderer{
		public:
			virtual ~BaseRenderer() = default;

			virtual void begin(SceneFramebuffer& target) = 0;
			virtual void end(SceneFramebuffer& target) = 0;

			virtual void render(Entity parent) = 0;
	};
}

#endif