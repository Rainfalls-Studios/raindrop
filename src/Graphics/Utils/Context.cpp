#include <Raindrop/Graphics/Utils/Context.hpp>

namespace Raindrop::Graphics::Utils{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger("Raindrop::Graphics::Utils"),
		_formats(*this),
		_stringToVulkan(*this)
	{}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}
	
	spdlog::logger& Context::logger(){
		return _logger;
	}

	FormatUtilities& Context::formats(){
		return _formats;
	}

	StringToVulkan& Context::stringToVulkan(){
		return _stringToVulkan;
	}
}