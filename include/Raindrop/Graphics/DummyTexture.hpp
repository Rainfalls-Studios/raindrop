#ifndef __RAINDROP_GRAPHICS_DUMMY_TEXTURE_HPP__
#define __RAINDROP_GRAPHICS_DUMMY_TEXTURE_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics{
	class DummyTexture : public Texture{
		public:
			DummyTexture(GraphicsContext& context);
			virtual ~DummyTexture() override;

		private:
			void createTexture();
	};
}

#endif