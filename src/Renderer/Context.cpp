#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	Context::Context(::Raindrop::Context& core) :
		core{core},
		allocationCallbacks{nullptr},
		window{*this},
		instance(*this),
		physicalDevice(*this),
		device(*this),
		queues(*this),
		commandPools(*this),
		swapchain(*this),
		baseFramebuffer(*this),
		white(*this),
		materials(*this),
		renderSystems(*this)
	{}
}