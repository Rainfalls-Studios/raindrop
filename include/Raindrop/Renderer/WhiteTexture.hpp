#ifndef __RAINDROP_RENDERER_WHITE_TEXTURE_HPP__
#define __RAINDROP_RENDERER_WHITE_TEXTURE_HPP__

#include "common.hpp"
#include "Texture/Texture.hpp"

namespace Raindrop::Renderer{
	class WhiteTexture{
		public:
			WhiteTexture(Context& context);

			const std::shared_ptr<Texture::Texture>& get() const;

		private:
			Context& _context;
			std::shared_ptr<Texture::Texture> _texture;
	};
}

#endif