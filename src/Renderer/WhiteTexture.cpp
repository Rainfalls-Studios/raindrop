#include <Raindrop/Renderer/WhiteTexture.hpp>

namespace Raindrop::Renderer{
	WhiteTexture::WhiteTexture(Context& context) : _context{context}{

		static constexpr uint32_t color = 0xFFFFFFFF;

		Texture::Texture::ConstructData data;
		data.width = 1;
		data.height = 1;
		data.channels = 4;
		data.data = &color;

		_texture = std::make_shared<Texture::Texture>(context, data);
	}

	const std::shared_ptr<Texture::Texture>& WhiteTexture::get() const{
		return _texture;
	}

}