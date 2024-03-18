#ifndef __RAINDROP_RENDERER_TEXTURE_LOADER_HPP__
#define __RAINDROP_RENDERER_TEXTURE_LOADER_HPP__

#include "common.hpp"
#include <Raindrop/Core/Asset/Loader.hpp>

namespace Raindrop::Renderer::Texture{
	class Loader : public ::Raindrop::Core::Asset::Loader{
		public:
			using Asset = ::Raindrop::Core::Asset::Asset;

			Loader(Context &context);
			virtual ~Loader() override;

			virtual std::shared_ptr<Asset> load(const Path& path) override;
		
		private:
			Context& _context;
			std::list<std::shared_ptr<Texture>> _textures;
	};
}

#endif