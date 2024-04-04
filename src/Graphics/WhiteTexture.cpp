#include <Raindrop/Graphics/WhiteTexture.hpp>

namespace Raindrop::Graphics{
	WhiteTexture::WhiteTexture(Context& context) : _context{context}{

		static constexpr uint32_t color = 0xFFFFFFFF;

		Textures::Texture::ConstructData data;
		data.width = 1;
		data.height = 1;
		data.channels = 4;
		data.data = &color;

		_texture = std::make_shared<Textures::Texture>(context, data);
	}

	const std::shared_ptr<Textures::Texture>& WhiteTexture::get() const{
		return _texture;
	}

}