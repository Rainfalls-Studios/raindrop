#ifndef __RAINDROP_SCENES_CONTEXT_HPP__
#define __RAINDROP_SCENES_CONTEXT_HPP__

#include "pch.pch"
#include "Scene.hpp"

namespace Raindrop::Scenes{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;

		void createLogger();
		void initialize();
	};
}

#endif