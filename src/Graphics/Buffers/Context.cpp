#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Buffers{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger(spdlog::stdout_logger_mt("Raindrop::Graphics::Buffers")){
		
		_logger->info("Loading Graphics Buffers context...");
		_defaultLayout = std::make_unique<VertexLayout>(*this);
		_defaultLayout->addAttribute("position", 0, VK_FORMAT_R32G32B32_SFLOAT);
		_defaultLayout->addAttribute("normal", 0, VK_FORMAT_R32G32B32_SFLOAT);
		_defaultLayout->addAttribute("UV", 0, VK_FORMAT_R32G32_SFLOAT);
		_defaultLayout->addAttribute("color", 0, VK_FORMAT_R32G32B32_SFLOAT);
		_logger->info("Graphics Buffers context loaded without any critical error");
	}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}

	VertexLayout& Context::defaultLayout(){
		return *_defaultLayout;
	}
}