#ifndef __RAINDROP_GRAPHICS_IMAGE_LOADER_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_LOADER_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadImages(const YAML::Node& node);
			void loadImageViews(const YAML::Node& node);

		private:
			Context& _context;
	};
}

#endif