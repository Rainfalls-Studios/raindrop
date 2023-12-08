#include <Raindrop/Graphics/RenderPass/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

#define LOGGER_NAME "Raindrop::Graphics::RenderPass"

namespace Raindrop::Graphics::RenderPass{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger{spdlog::stdout_logger_mt(LOGGER_NAME)}{
		
		_loader = std::make_unique<Loader>(*this);
		_renderPassManager = std::make_unique<RenderPassManager>(*this);
	}

	Context::~Context(){
		_renderPassManager.reset();
		_loader.reset();
	}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	RenderPassManager& Context::renderPassManager(){
		return *_renderPassManager;
	}

	Loader& Context::loader(){
		return *_loader;
	}
}