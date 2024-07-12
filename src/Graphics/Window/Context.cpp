#include <Raindrop/Graphics/Window/Context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics::Window{
	void Context::createLogger(){
		logger = spdlog::stdout_color_st("Raindrop::Graphics::Window");
	}
}