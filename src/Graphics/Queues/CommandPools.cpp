#include <Raindrop/Graphics/Queues/CommandPools.hpp>

namespace Raindrop::Graphics::Queues{
	CommandPools::CommandPools(Context& context) : 
		singleUseTransfert(context),
		graphics(context){}
}