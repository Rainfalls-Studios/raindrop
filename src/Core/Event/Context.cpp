#include <Raindrop/Core/Event/Context.hpp>

namespace Raindrop::Core::Event{
    Context::Context(Core::Context& core) :
        core{core},
        logger("Event"){}
}