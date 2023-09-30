#ifndef __RAINDROP_GRAPHICS_QUEUES_GRAPHICS_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_QUEUES_GRAPHICS_FAMILY_HPP__

#include <Raindrop/Graphics/Queues/common.hpp>
#include <Raindrop/Graphics/Queues/GraphicsCommandPool.hpp>
#include <Raindrop/Graphics/Queues/GraphicsCommandBuffers.hpp>

namespace Raindrop::Graphics::Queues{
	struct GraphicsFamily{
		GraphicsFamily(GraphicsContext& context);

		uint32_t familyIndex;
		GraphicsCommandPool commandPool;
		GraphicsCommandBuffers commandBuffers;
		VkQueue queue;

		private:
			GraphicsContext& _context;
	};
}

#endif