#ifndef __RAINDROP_GRAPHICS_SYNC_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_SYNC_CONTEXT_HPP__

#include "QueueManager.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Sync{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		QueueManager manager;
		
		void createLogger();
	};
}

#endif