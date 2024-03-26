#include <Raindrop/Renderer/Queues/CommandPools.hpp>

namespace Raindrop::Renderer::Queues{
	CommandPools::CommandPools(Context& context) : 
		singleUseTransfert(context),
		graphics(context){}
}