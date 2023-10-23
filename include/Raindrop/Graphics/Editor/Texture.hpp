#ifndef __RAINDROP_GRAPHICS_EDITOR_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_TEXTURE_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Editor/Image.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics::Editor{
	class Texture : public Image, public Graphics::Texture{
		public:
			Texture(EditorContext& context, const std::filesystem::path& path);
			virtual ~Texture() override;

			virtual uint32_t width() const override;
			virtual uint32_t height() const override;
	};
}

#endif