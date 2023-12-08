#include <Raindrop/Graphics/Builders/Context.hpp>

#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Builders{
	Context::Context(Graphics::Context& context) :
		_graphics{context},
		_logger{spdlog::stdout_logger_mt("Raindrop::Graphics::Builders")}
	{}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}
}