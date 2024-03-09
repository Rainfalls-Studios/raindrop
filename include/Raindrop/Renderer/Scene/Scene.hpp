#ifndef __RAINDROP_RENDERER_SCENE_SCENE_HPP__
#define __RAINDROP_RENDERER_SCENE_SCENE_HPP__

#include "common.hpp"
#include "Context.hpp"

namespace Raindrop::Renderer::Scene{
	class Scene{
		public:
			Scene(::Raindrop::Renderer::Context& renderer);
			~Scene();

		private:
			::Raindrop::Renderer::Context& _renderer;
			Context _context;
	};
}

#endif