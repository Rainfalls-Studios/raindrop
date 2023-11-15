#include <Raindrop/Graphics/Utils/Context.hpp>
#include <spdlog/sinks/stdout_sinks.h>

namespace Raindrop::Graphics::Utils{
	Context::Context(Graphics::Context& graphics) :
		_graphics{graphics},
		_logger(spdlog::stdout_logger_mt("Raindrop::Graphics::Utils"))
	{
		
		_logger->info("Loading Graphics Utils context...");

		_formats = std::make_unique<FormatUtilities>(*this);
		_stringToVulkan = std::make_unique<StringToVulkan>(*this);
		
		_logger->info("Graphics Utils context loaded without any critical error");
	}
	
	Graphics::Context& Context::graphics(){
		return _graphics;
	}
	
	spdlog::logger& Context::logger(){
		return *_logger;
	}

	FormatUtilities& Context::formats(){
		return *_formats;
	}

	StringToVulkan& Context::stringToVulkan(){
		return *_stringToVulkan;
	}
}