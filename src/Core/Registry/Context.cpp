#include <Raindrop/Core/Registry/Context.hpp>

namespace Raindrop::Core::Registry{
    Context::Context(Core::Context& core) :
        core{core},
        logger("Core::Registry"){}
}