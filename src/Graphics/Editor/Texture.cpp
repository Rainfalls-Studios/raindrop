#include <Raindrop/Graphics/Editor/Texture.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

namespace Raindrop::Graphics::Editor{
	Texture::Texture(EditorContext& context, const std::filesystem::path& path) : Image(context), Graphics::Texture(context.context, path){
		Image::setTexture(Texture::_sampler, Texture::_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	Texture::~Texture(){
		Image::removeTexture();
	}

	uint32_t Texture::width() const{
		return Texture::_width;
	}

	uint32_t Texture::height() const{
		return Texture::_height;
	}
}