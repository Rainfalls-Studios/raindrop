#ifndef __RAINDROP_INTERNAL_GRAPHICS_QUEUES_COMMAND_POOLS_GRAPHICS_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_QUEUES_COMMAND_POOLS_GRAPHICS_HPP__

#include "CommandPool.hpp"

namespace Raindrop::Internal::Graphics::Queues{
	class Graphics : public CommandPool{
		public:
			Graphics(Context& context);

		protected:
			virtual VkCommandPoolCreateInfo createInfo() const override;
	};
}

#endif