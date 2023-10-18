#include <Raindrop/Graphics/Buffers/Context.hpp>

namespace Raindrop::Graphics::Buffers{
	Context::Context(Graphics::Context& graphics) : 
		graphics{graphics},
		logger("Raindrop::Graphics::Buffers"),
		defaultLayout(*this){
		
		defaultLayout.addAttribute("position", 0, VK_FORMAT_R32G32B32_SFLOAT);
		defaultLayout.addAttribute("normal", 0, VK_FORMAT_R32G32B32_SFLOAT);
		defaultLayout.addAttribute("UV", 0, VK_FORMAT_R32G32_SFLOAT);
		defaultLayout.addAttribute("color", 0, VK_FORMAT_R32G32B32_SFLOAT);
	}
}