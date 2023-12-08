#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_LOADER_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_LOADER_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::RenderPass{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadRenderPasses(const YAML::Node& node);
			
		private:
			Context& _context;
	};
}

#endif