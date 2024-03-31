#ifndef __RAINDROP_QUEUES_COMMAND_POOLS_SINGLE_USE_TRANSFERT_HPP__
#define __RAINDROP_QUEUES_COMMAND_POOLS_SINGLE_USE_TRANSFERT_HPP__

#include "CommandPool.hpp"

namespace Raindrop::Graphics::Queues{
	class SingleUseTransfer : public CommandPool{
		public:
			SingleUseTransfer(Context& context);

		protected:
			virtual VkCommandPoolCreateInfo createInfo() const override;
	};
}

#endif