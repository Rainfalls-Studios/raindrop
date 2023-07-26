#ifndef __RAINDROP_GRAPHICS_EDITOR_FONTS_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_FONTS_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class Fonts{
		public:
			Fonts();
			~Fonts();
			ImFont* get(const std::string& name);
			void load(const std::string& name, float size, const std::filesystem::path& path);

		private:
			std::unordered_map<std::string, ImFont*> _nameToFont;
	};
}

#endif