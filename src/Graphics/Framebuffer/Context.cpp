#include <Raindrop/Graphics/Framebuffer/Context.hpp>

#include <spdlog/sinks/stdout_sinks.h>

#define LOGGER_NAME "Raindrop::Graphics::Framebuffer"

namespace Raindrop::Graphics::Framebuffer{
	Context::Context(Graphics::Context& graphics) : 
		_graphics{graphics},
		_logger{spdlog::stderr_logger_mt(LOGGER_NAME)}{

		_manager = std::make_unique<FramebufferManager>(*this);
		_loader = std::make_unique<Loader>(*this);

	}

	Context::~Context(){
		
		_loader.reset();
		_manager.reset();

		spdlog::drop(LOGGER_NAME);
	}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	FramebufferManager& Context::manager(){
		return *_manager;
	}

	Loader& Context::loader(){
		return *_loader;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}
}