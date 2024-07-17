#ifndef __RAINDROP_EVENTS_CONTEXT_HPP__
#define __RAINDROP_EVENTS_CONTEXT_HPP__

#include "common.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Events{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		void initialize();
		void createLogger();
	};
}

#endif