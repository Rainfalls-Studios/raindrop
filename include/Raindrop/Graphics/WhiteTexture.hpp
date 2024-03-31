#ifndef __RAINDROP_GRAPHICS_WHITE_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_WHITE_TEXTURE_HPP__

#include "common.hpp"
#include "Textures/Texture.hpp"

namespace Raindrop::Graphics{
	class WhiteTexture{
		public:
			WhiteTexture(Context& context);

			const std::shared_ptr<Textures::Texture>& get() const;

		private:
			Context& _context;
			std::shared_ptr<Textures::Texture> _texture;
	};
}

#endif