#ifndef __RAINDROP_GRAPHICS_QUEUES_PRESENT_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_QUEUES_PRESENT_FAMILY_HPP__

#include <Raindrop/Graphics/Queues/common.hpp>

namespace Raindrop::Graphics::Queues{
	struct PresentFamily{
		PresentFamily(GraphicsContext& context);

		uint32_t familyIndex;
		VkQueue queue;

		private:
			GraphicsContext& _context;
	};
}

#endif