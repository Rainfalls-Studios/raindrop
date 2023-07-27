#include <Raindrop/Graphics/factory/TextureFactory.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics::Factory{
	TextureFactory::TextureFactory(GraphicsContext& context) : _context{context}{

	}

	TextureFactory::~TextureFactory(){

	}

	std::shared_ptr<Core::Asset::Asset> TextureFactory::createAsset(const std::filesystem::path& path){
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(_context, path);
		_textures.push_back(texture);

		return std::static_pointer_cast<Core::Asset::Asset>(texture);
	}

	void TextureFactory::destroyAsset(std::shared_ptr<Core::Asset::Asset> asset){
		_textures.remove(std::static_pointer_cast<Texture>(asset));
	}

	void TextureFactory::registerExtensions(const std::shared_ptr<TextureFactory>& factory){
		auto& assetManager = _context.context.assetManager;

		assetManager.linkFactory(".jpg", factory);
		assetManager.linkFactory(".jpeg", factory);
		assetManager.linkFactory(".png", factory);
		assetManager.linkFactory(".bmp", factory);
	}
}