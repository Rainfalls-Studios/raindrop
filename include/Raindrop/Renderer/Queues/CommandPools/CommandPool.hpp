#ifndef __RAINDROP_QUEUES_COMMAND_POOLS_COMMAND_POOL_HPP__
#define __RAINDROP_QUEUES_COMMAND_POOLS_COMMAND_POOL_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Queues{
	class CommandPool{
		public:
			CommandPool(Context& context);
			~CommandPool();

			VkCommandPool get() const;

		private:
			VkCommandPool _pool;

		protected:
			Context& _context;
			virtual VkCommandPoolCreateInfo createInfo() const = 0;

			void create();
	};
}

#endif