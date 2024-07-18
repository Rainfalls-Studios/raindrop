#include <Raindrop/Events/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Events{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Events");
	}

	void Context::initialize(){
		createLogger();

		manager.prepare(*this);
		manager.initialize();
	}
}