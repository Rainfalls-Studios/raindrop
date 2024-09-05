#include "Raindrop/Graphics/Context.hpp"
#include "Raindrop/Core/Context.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger{spdlog::stdout_color_st("Raindrop::Graphics")},
		_window(*this, "Raindrop", 1080, 720
	{}

	Context::~Context(){

	}

	std::shared_ptr<spdlog::logger>& Context::getLogger() noexcept{
		return _logger;
	}

	const std::shared_ptr<spdlog::logger>& Context::getLogger() const noexcept{
		return _logger;
	}
}