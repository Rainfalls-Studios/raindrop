#ifndef __RAINDROP_GRAPHICS_FACTORY_TEXTURE_FACTORY_HPP__
#define __RAINDROP_GRAPHICS_FACTORY_TEXTURE_FACTORY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics::Factory{
	class TextureFactory : public Core::Asset::AssetFactory{
		public:
			TextureFactory(GraphicsContext& context);
			virtual ~TextureFactory() override;

			virtual std::shared_ptr<Core::Asset::Asset> createAsset(const std::filesystem::path& path) override;
			virtual void destroyAsset(std::shared_ptr<Core::Asset::Asset> asset) override;

			void registerExtensions(const std::shared_ptr<TextureFactory>& factory);

		private:
			GraphicsContext& _context;
			std::list<std::shared_ptr<Texture>> _textures;
	};
}

#endif