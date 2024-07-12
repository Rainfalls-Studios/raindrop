#include <Raindrop/Graphics/Sync/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics::Sync{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Graphics::Sync");
	}
}
