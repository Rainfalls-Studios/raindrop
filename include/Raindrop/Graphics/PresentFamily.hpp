#ifndef __RAINDROP_GRAPHICS_PRESENT_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_PRESENT_FAMILY_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	struct PresentFamily{
		PresentFamily(GraphicsContext& context);

		uint32_t familyIndex;
		VkQueue queue;

		private:
			GraphicsContext& _context;
	};
}

#endif