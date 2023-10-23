#ifndef __RAINDROP_CORE_ASSET_CONTEXT_HPP__
#define __RAINDROP_CORE_ASSET_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Asset/common.hpp>

namespace Raindrop::Core::Asset{
	struct Context{
		Core::Context& core;
		spdlog::logger logger;

		Context(Core::Context& core);
	};
}

#endif