#ifndef __RAINDROP_RENDERER_QUEUES_COMMAND_POOLS_HPP__
#define __RAINDROP_RENDERER_QUEUES_COMMAND_POOLS_HPP__

#include "common.hpp"
#include "CommandPools/SingleUseTransfert.hpp"
#include "CommandPools/Graphics.hpp"

namespace Raindrop::Renderer::Queues{
	struct CommandPools{
		CommandPools(Context& context);

		SingleUseTransfer singleUseTransfert;
		Graphics graphics;
	};
}

#endif