#ifndef __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__
#define __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
    struct Context{
        Core::Context& core;
        spdlog::logger logger;

        Context(Core::Context& core);
    };
}

#endif