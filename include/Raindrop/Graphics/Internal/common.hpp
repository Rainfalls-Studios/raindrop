#ifndef __RAINDROP_GRAPHICS_INTERNAL_COMMON_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_COMMON_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Internal/SwapchainSupport.hpp>
#include <Raindrop/Graphics/Internal/structs.hpp>

namespace Raindrop::Graphics::Internal{
	struct Context;
	struct SwapchainSupport;
	struct QueueProperties;
	class Instance;
	class PhysicalDevice;
	class Device;
	class Swapchain;
	class Window;
	class QueueHandler;
	class QueueFamily;
	class Queue;
	class CommandPools;
	class CommandPool;
	class CommandBuffer;
}


#endif