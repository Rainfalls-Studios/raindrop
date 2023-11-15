#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Shaders{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger{spdlog::stdout_logger_mt("Raindrop::Graphics::Shaders")}{
		
		_logger->info("Initializing Graphics Shaders context...");
		_compiler = std::make_unique<ShaderCompiler>(*this);

		_shaderFactory = _graphics.core().assetManager().registerFactory<ShaderFactory, Context&>(*this);

		_logger->info("Graphics Shaders context initialized without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Graphics Shaders context...");
		_shaderFactory.reset();
		_compiler.reset();
		_logger->info("Graphics Shaders context terminated without any critical error");
	}

	Graphics::Context& Context::graphics(){
		return _graphics;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}

	ShaderCompiler& Context::compiler(){
		return *_compiler;
	}

	ShaderFactory& Context::shaderFactory(){
		return *_shaderFactory;
	}
}