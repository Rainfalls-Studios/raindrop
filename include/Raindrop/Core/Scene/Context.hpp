#ifndef __RAINDROP_CORE_SCENE_CONTEXT_HPP__
#define __RAINDROP_CORE_SCENE_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Scene/common.hpp>

namespace Raindrop::Core::Scene{
	struct Context{
		Core::Context& core;
		spdlog::logger logger;

		Context(Core::Context& core);
	};
}

#endif