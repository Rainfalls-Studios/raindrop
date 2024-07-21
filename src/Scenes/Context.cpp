#include <Raindrop/Scenes/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Raindrop::Scenes{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Scenes");
	}

	void Context::initialize(){
		createLogger();
	}
}