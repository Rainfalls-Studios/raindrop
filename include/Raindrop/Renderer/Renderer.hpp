#ifndef __RAINDROP_RENDERER_RENDERER_HPP__
#define __RAINDROP_RENDERER_RENDERER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer{
	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			void render();
			void events();

		private:
			Context* _context;
	};
}

#endif