#ifndef __RAINDROP_GRAPHICS_EDITOR_ICON_ATLAS_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_ICON_ATLAS_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>

namespace Raindrop::Graphics::Editor{
	class IconAtlas{
		public:
			IconAtlas(EditorContext& context);
			~IconAtlas();

			void loadFromXML(tinyxml2::XMLElement* element);

			Icon& get(const std::string& name);
			Icon& create(const std::string& name, const std::filesystem::path& atlas, const glm::u32vec2& uv1, const glm::u32vec2& uv2);

		private:
			EditorContext& _context;
			std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> _pathToTexture;
			std::unordered_map<std::string, std::shared_ptr<Icon>> _nameToIcon;

			void loadIconFromXML(tinyxml2::XMLElement* element);

			Texture& getAtlas(const std::filesystem::path& path);
	};
}

#endif