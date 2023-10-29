#include <Raindrop/Core/Event/Context.hpp>

namespace Raindrop::Core::Event{
    Context::Context(Core::Context& core) :
        _core{core},
        _logger("Raindrop::Core::Event")
    {}

    Core::Context& Context::core(){
        return _core;
    }

    spdlog::logger& Context::logger(){
        return _logger;
    }
}