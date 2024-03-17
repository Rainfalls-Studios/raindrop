#ifndef __RAINDROP_RENDERER_QUEUES_COMMAND_POOLS_GRAPHICS_HPP__
#define __RAINDROP_RENDERER_QUEUES_COMMAND_POOLS_GRAPHICS_HPP__

#include "CommandPool.hpp"

namespace Raindrop::Renderer::Queues{
	class Graphics : public CommandPool{
		public:
			Graphics(Context& context);

		protected:
			virtual VkCommandPoolCreateInfo createInfo() const override;
	};
}

#endif