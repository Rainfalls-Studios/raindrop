// #include <Raindrop/Graphics/DummyTexture.hpp>

// namespace Raindrop::Graphics{
// 	DummyTexture::DummyTexture(GraphicsContext& context) : Texture(context){
// 		createTexture();
// 	}

// 	DummyTexture::~DummyTexture(){}

// 	void DummyTexture::createTexture(){
// 		uint32_t pixel = 0xFFFFFFFF;
// 		Texture::createImageFromPtr(static_cast<void*>(&pixel), 1, 1, 4);
// 	}
// }