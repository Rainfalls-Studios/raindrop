#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger("Raindrop::Graphics::Buffers"),
		_defaultLayout(*this){
		
		_defaultLayout.addAttribute("position", 0, VK_FORMAT_R32G32B32_SFLOAT);
		_defaultLayout.addAttribute("normal", 0, VK_FORMAT_R32G32B32_SFLOAT);
		_defaultLayout.addAttribute("UV", 0, VK_FORMAT_R32G32_SFLOAT);
		_defaultLayout.addAttribute("color", 0, VK_FORMAT_R32G32B32_SFLOAT);
	}

	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}
	
	spdlog::logger& Context::logger(){
		return _logger;
	}

	VertexLayout& Context::defaultLayout(){
		return _defaultLayout;
	}
}