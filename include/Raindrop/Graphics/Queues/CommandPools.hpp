#ifndef __RAINDROP_GRAPHICS_QUEUES_COMMAND_POOLS_HPP__
#define __RAINDROP_GRAPHICS_QUEUES_COMMAND_POOLS_HPP__

#include "common.hpp"
#include "CommandPools/SingleUseTransfert.hpp"
#include "CommandPools/Graphics.hpp"

namespace Raindrop::Graphics::Queues{
	struct CommandPools{
		CommandPools(Context& context);

		SingleUseTransfer singleUseTransfert;
		Graphics graphics;
	};
}

#endif