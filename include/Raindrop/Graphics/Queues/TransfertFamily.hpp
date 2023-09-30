#ifndef __RAINDROP_GRAPHICS_QUEUES_TRANSFERT_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_QUEUES_TRANSFERT_FAMILY_HPP__

#include <Raindrop/Graphics/Queues/common.hpp>
#include <Raindrop/Graphics/Queues/TransfertCommandPool.hpp>

namespace Raindrop::Graphics::Queues{
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