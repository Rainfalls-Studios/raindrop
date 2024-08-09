#include <Raindrop/Events/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Events{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Events");
	}

	void Context::initialize(Raindrop::Context& raindrop){
		this->raindrop = &raindrop;
		createLogger();

		manager.prepare(*this);
		manager.initialize();

		inputs.prepare(*this);
		inputs.initialize();
	}
}