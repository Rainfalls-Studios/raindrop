#include <Raindrop/Graphics/Editor/IconAtlas.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>
#include <Raindrop/Graphics/Editor/Texture.hpp>
#include <Raindrop/Graphics/Editor/Icon.hpp>

namespace Raindrop::Graphics::Editor{
	IconAtlas::IconAtlas(EditorContext& context) : _context{context}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Editor.IconAtlas");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");
	}

	IconAtlas::~IconAtlas(){

	}
	
	void IconAtlas::loadFromXML(tinyxml2::XMLElement* element){
		CLOG(INFO, "Engine.Graphics.Editor.IconAtlas") << "Loading icon(s) from xml element...";

		for (auto icon = element->FirstChildElement("Icon"); icon!=nullptr; icon = icon->NextSiblingElement("Icon")){
			try{
				loadIconFromXML(icon);
			} catch (const std::exception &e){
				CLOG(INFO, "Engine.Graphics.Editor.IconAtlas") << "Failed to load icon";
				continue;
			}
		}

		CLOG(INFO, "Engine.Graphics.Editor.IconAtlas") << "Icon(s) loaded with success";
	}

	Icon& IconAtlas::get(const std::string& name){
		auto it = _nameToIcon.find(name);
		if (it == _nameToIcon.end()){
			CLOG(WARNING, "Engine.Graphics.Editor.IconAtlas") << "Cannot get \"" << name << "\" icon, unregistred icon";
			throw std::runtime_error("Unregistred icon");
		}
		return *it->second;
	}

	void IconAtlas::loadIconFromXML(tinyxml2::XMLElement* element){
		const char* name;
		if (element->QueryStringAttribute("Name", &name) != tinyxml2::XML_SUCCESS){
			CLOG(WARNING, "Engine.Graphics.Editor.IconAtlas") << "Cannot create icon from xml without a \"Name\" string attribute";
			throw std::runtime_error("Missing \"Name\" attribute");
		}

		const char* atlas;
		if (element->QueryStringAttribute("Atlas", &atlas) != tinyxml2::XML_SUCCESS){
			CLOG(WARNING, "Engine.Graphics.Editor.IconAtlas") << "Cannot create icon from xml without a \"Atlas\" string attribute";
			throw std::runtime_error("Missing \"Atlas\" attribute");
		}

		glm::u32vec2 uv1 = glm::u32vec2(0, 0);
		glm::u32vec2 uv2 = glm::u32vec2(1, 1);

		uv1.x = static_cast<uint32_t>(element->IntAttribute("UV1x", 0));
		uv1.y = static_cast<uint32_t>(element->IntAttribute("UV1y", 0));
		uv2.x = static_cast<uint32_t>(element->IntAttribute("UV2x", 0));
		uv2.y = static_cast<uint32_t>(element->IntAttribute("UV2y", 0));

		create(name, _context.context.context.registry.formatString(atlas), uv1, uv2);
	}

	Icon& IconAtlas::create(const std::string& name, const std::filesystem::path& atlas, const glm::u32vec2& uv1, const glm::u32vec2& uv2){
		Texture& texture = getAtlas(atlas);

		std::shared_ptr<Icon> icon = std::make_shared<Icon>();
		
		icon->_id = texture.texture();
		icon->_uv1 = ImVec2(uv1.x / static_cast<float>(texture.width()), uv1.y / static_cast<float>(texture.height()));
		icon->_uv2 = ImVec2(uv2.x / static_cast<float>(texture.width()), uv2.y / static_cast<float>(texture.height()));
		
		_nameToIcon[name] = icon;

		return *icon;
	}

	Texture& IconAtlas::getAtlas(const std::filesystem::path& path){
		auto it = _pathToTexture.find(path);
		if (it != _pathToTexture.end())
			return *it->second;

		std::shared_ptr<Texture> texture = std::make_shared<Texture>(_context, path);
		_pathToTexture[path] = texture;
		return *texture;
	}
}