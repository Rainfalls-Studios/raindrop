#include <Raindrop/Graphics/Formats/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

#define LOGGER_NAME "Raindrop::Graphics::Formats"

namespace Raindrop::Graphics::Formats{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger{spdlog::stderr_logger_mt(LOGGER_NAME)}{

		_registry = std::make_unique<FormatRegistry>(*this);
		_loader = std::make_unique<Loader>(*this);
	}

	Context::~Context(){
		_loader.reset();
		_registry.reset();
		spdlog::drop(LOGGER_NAME);
	}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	Loader& Context::loader(){
		return *_loader;
	}

	FormatRegistry& Context::registry(){
		return *_registry;
	}
}