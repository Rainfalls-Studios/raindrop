#include <Raindrop/Graphics/Buffers/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Buffers{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger(spdlog::stdout_logger_mt("Raindrop::Graphics::Buffers")){
		
		_logger->info("Loading Graphics Buffers context...");

		_layoutRegistry = std::make_unique<VertexLayoutRegistry>(*this);
		_loader = std::make_unique<Loader>(*this);

		auto& defaultLayout = _layoutRegistry->get("default");
		
		defaultLayout = std::make_unique<VertexLayout>(*this);
		defaultLayout->addAttribute("position", 0, VK_FORMAT_R32G32B32_SFLOAT, Position);
		defaultLayout->addAttribute("normal", 0, VK_FORMAT_R32G32B32_SFLOAT, Normal);
		defaultLayout->addAttribute("UV", 0, VK_FORMAT_R32G32_SFLOAT, Uv);
		defaultLayout->addAttribute("color", 0, VK_FORMAT_R32G32B32_SFLOAT, Color);

		_logger->info("Graphics Buffers context loaded without any critical error");
	}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}

	VertexLayoutRegistry& Context::vertexdLayoutRegistry(){
		return *_layoutRegistry;
	}

	Loader& Context::loader(){
		return *_loader;
	}
}