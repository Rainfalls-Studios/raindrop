#ifndef __RAINDROP_CORE_EVENT_CONTEXT_HPP__
#define __RAINDROP_CORE_EVENT_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Event/common.hpp>

namespace Raindrop::Core::Event{
    struct Context{
        Core::Context& core;
        spdlog::logger logger;

        Context(Core::Context& context);
    };
}

#endif