#ifndef __RAINDROP_GRAPHICS_SYNC_CONTEXT_HPP__
#define __RAINDROP_GRAPHICS_SYNC_CONTEXT_HPP__

#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Sync{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		
		void createLogger();
	};
}

#endif