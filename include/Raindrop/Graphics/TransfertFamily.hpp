#ifndef __RAINDROP_GRAPHICS_TRANSFERT_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_TRANSFERT_FAMILY_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/TransfertCommandPool.hpp>

namespace Raindrop::Graphics{
	struct TransfertFamily{
		TransfertFamily(GraphicsContext& context);

		uint32_t familyIndex;
		TransfertCommandPool commandPool;
		VkQueue queue;

		private:
			GraphicsContext& _context;
	};
}

#endif