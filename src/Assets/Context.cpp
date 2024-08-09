#include <Raindrop/Assets/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Assets{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Assets");
	}

	void Context::initialize(Raindrop::Context& raindrop){
		this->raindrop = &raindrop;
		createLogger();

		manager.prepare(*this)
			.initialize();
	}
	
}