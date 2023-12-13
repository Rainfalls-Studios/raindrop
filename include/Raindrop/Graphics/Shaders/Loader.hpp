#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_LOADER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_LOADER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadGraphicsPipelines(const YAML::Node& node);
			void loadPipelineLayouts(const YAML::Node& node);
		
		private:
			Context& _context;
	};
}

#endif