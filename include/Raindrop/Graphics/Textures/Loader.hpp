#ifndef __RAINDROP_GRAPHICS_TEXTURE_LOADER_HPP__
#define __RAINDROP_GRAPHICS_TEXTURE_LOADER_HPP__

#include "common.hpp"
#include <Raindrop/Core/Assets/Loader.hpp>

namespace Raindrop::Graphics::Textures{
	class Loader : public ::Raindrop::Core::Assets::Loader{
		public:
			using Asset = ::Raindrop::Core::Assets::Asset;

			Loader(Context &context);
			virtual ~Loader() override;

			virtual std::shared_ptr<Asset> load(const Path& path) override;
			void registerAsset(const Path& path, const std::shared_ptr<Asset>& asset) override;
		
		private:
			Context& _context;
			std::list<std::shared_ptr<Texture>> _textures;
	};
}

#endif