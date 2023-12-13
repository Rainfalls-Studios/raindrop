#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Engine.hpp>
#include <spdlog/sinks/stdout_sinks.h>

#define LOGGER_NAME "Raindrop::Graphics"

namespace Raindrop::Graphics{
	Context::Context(Core::Engine& core) :
		_core{core},
		_logger(spdlog::stdout_logger_mt(LOGGER_NAME)){
			
		_logger->info("Loading Graphics context...");
		_internal = std::make_unique<Internal::Context>(*this);
		_utils = std::make_unique<Utils::Context>(*this);
		_renderPass = std::make_unique<RenderPass::Context>(*this);
		_shaders = std::make_unique<Shaders::Context>(*this);
		_image = std::make_unique<Image::Context>(*this);
		_framebuffer = std::make_unique<Framebuffer::Context>(*this);
		_formats = std::make_unique<Formats::Context>(*this);
		_loader = std::make_unique<Loader>(*this);
		_logger->info("Graphics context loaded without any critical error");
	}

	Context::~Context(){
		_logger->info("Terminating Graphics context...");
		_loader.reset();
		_formats.reset();
		_framebuffer.reset();
		_image.reset();
		_shaders.reset();
		_renderPass.reset();
		_utils.reset();
		_internal.reset();
		_logger->info("Graphics context terminated without any critical error");
		spdlog::drop(LOGGER_NAME);
	}

	Core::Engine& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return *_logger;
	}
	
	VkAllocationCallbacks*& Context::allocationCallbacks(){
		return _allocationCallbacks;
	}

	Internal::Context& Context::internal(){
		return *_internal;
	}

	Utils::Context& Context::utils(){
		return *_utils;
	}

	Shaders::Context& Context::shaders(){
		return *_shaders;
	}

	Image::Context& Context::image(){
		return *_image;
	}

	Loader& Context::loader(){
		return *_loader;
	}

	RenderPass::Context& Context::renderPass(){
		return *_renderPass;
	}

	Framebuffer::Context& Context::framebuffer(){
		return *_framebuffer;
	}

	Formats::Context& Context::formats(){
		return *_formats;
	}
}