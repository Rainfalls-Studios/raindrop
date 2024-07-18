#ifndef __RAINDROP_EVENTS_CONTEXT_HPP__
#define __RAINDROP_EVENTS_CONTEXT_HPP__

#include "common.hpp"
#include <spdlog/spdlog.h>
#include "Manager.hpp"

namespace Raindrop::Events{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		Manager manager;

		void initialize();
		void createLogger();
	};
}

#endif