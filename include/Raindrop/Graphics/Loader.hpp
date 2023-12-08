#ifndef __RAINDROP_GRAPHICS_LOADER_HPP__
#define __RAINDROP_GRAPHICS_LOADER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Loader{
		public:
			Loader(Context& context);
			~Loader();

			void loadFile(const std::filesystem::path& path);
			void loadNode(const YAML::Node& node);

		private:
			Context& _context;
	};
}

#endif