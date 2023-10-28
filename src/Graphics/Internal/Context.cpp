#include <Raindrop/Graphics/Internal/Context.hpp>

namespace Raindrop::Graphics::Internal{
	Context::Context(Graphics::Context& graphics) : 
		graphics{graphics},
		logger("Graphics::Internal"),
		window(*this),
		instance(*this),
		device(*this),
		queueHandler(*this),
		commandPools(*this),
		swapchain(*this){}

	const VkPhysicalDeviceLimits& Context::limits() const{
		return physicalDevice.limits();
	}
}