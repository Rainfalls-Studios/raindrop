#ifndef __RAINDROP_GRAPHICS_GRAPHICS_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_GRAPHICS_FAMILY_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/GraphicsCommandPool.hpp>
#include <Raindrop/Graphics/GraphicsCommandBuffers.hpp>

namespace Raindrop::Graphics{
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