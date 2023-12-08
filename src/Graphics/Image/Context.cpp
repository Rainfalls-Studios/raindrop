#include <Raindrop/Graphics/Image/Context.hpp>

#include <spdlog/sinks/stdout_sinks.h>

#define LOGGER_NAME "Raindrop::Graphics::Image"

namespace Raindrop::Graphics::Image{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger{spdlog::stdout_logger_mt(LOGGER_NAME)}
	{
		_logger->info("Initializing Context...");

		_imageManager = std::make_unique<ImageManager>(*this);
		_imageViewManager = std::make_unique<ImageViewManager>(*this);
		_loader = std::make_unique<Loader>(*this);

		_logger->info("Context initialized without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Context...");

		_loader.reset();
		_imageViewManager.reset();
		_imageManager.reset();

		_logger->info("Context terminated without any critical error");
		spdlog::drop(LOGGER_NAME);
	}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	ImageManager& Context::imageManager(){
		return *_imageManager;
	}

	ImageViewManager& Context::imageViewManager(){
		return *_imageViewManager;
	}

	Loader& Context::loader(){
		return *_loader;
	}
}