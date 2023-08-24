#include <Raindrop/Graphics/GraphicsContext.hpp>
#include <Raindrop/Graphics/Instance.hpp>
#include <Raindrop/Graphics/Device.hpp>
#include <Raindrop/Graphics/Window.hpp>
#include <Raindrop/Graphics/Texture.hpp>

namespace Raindrop::Graphics{
	GraphicsContext::GraphicsContext(Core::EngineContext& context) :
		context{context},
		gRegistry{},
		window{*this},
		instance{*this},
		device{*this},
		swapchain{*this},
		graphics{*this},
		present{*this},
		transfert{*this},
		layouts{*this},
		renderPasses{*this},
		descriptorPool{*this},
		dummyTexture{*this}{
	}

	GraphicsContext::~GraphicsContext(){}
}