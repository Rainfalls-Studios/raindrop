#ifndef __RAINDROP_RENDERER_RENDER_SYSTEM_RENDER_SYSTEM_HPP__
#define __RAINDROP_RENDERER_RENDER_SYSTEM_RENDER_SYSTEM_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::RenderSystem{
	class RenderSystem{
		public:
			RenderSystem() = default;
			virtual ~RenderSystem() = default;
	};
}

#endif