#ifndef __RAINDROP_SCENES_CONTEXT_HPP__
#define __RAINDROP_SCENES_CONTEXT_HPP__

#include "pch.pch"
#include "Scene.hpp"
#include "../Context.hpp"

namespace Raindrop::Scenes{
	struct Context{
		std::shared_ptr<spdlog::logger> logger;
		Raindrop::Context* core;

		void createLogger();
		void initialize(Raindrop::Context& core);
	};
}

#endif