#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer{
	Context::Context(::Raindrop::Context& core) :
		core{core},
		allocationCallbacks{nullptr},
		window{*this},
		instance(*this),
		device(*this),
		queues(*this),
		commandPools(*this),
		swapchain(*this),
		scene(*this)
	{}
}