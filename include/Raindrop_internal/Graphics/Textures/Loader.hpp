#ifndef __RAINDROP_INTERNAL_GRAPHICS_TEXTURE_LOADER_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_TEXTURE_LOADER_HPP__

#include "common.hpp"
#include <Raindrop/Asset.hpp>

namespace Raindrop::Internal::Graphics::Textures{
	class Loader : public ::Raindrop::Asset::Loader{
		public:
			using Asset = ::Raindrop::Asset;

			Loader(Context &context);
			virtual ~Loader() override;

			virtual void collectGarbage() override;
			virtual std::shared_ptr<Asset> load(const Path& path) override;
		
		private:
			Context& _context;
			std::list<std::shared_ptr<Texture>> _textures;
	};
}

#endif