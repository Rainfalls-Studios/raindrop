#include <Raindrop/Graphics/Core/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics::Core{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Graphics::Core");
	}
}