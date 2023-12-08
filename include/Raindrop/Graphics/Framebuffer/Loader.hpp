#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_LOADER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_LOADER_HPP__

#include <Raindrop/Graphics/Framebuffer/common.hpp>

namespace Raindrop::Graphics::Framebuffer{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadFramebuffers(const YAML::Node& node);

		private:
			Context& _context;
	};
}

#endif