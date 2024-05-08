#include <Raindrop_internal/Graphics/Queues/CommandPools.hpp>

namespace Raindrop::Internal::Graphics::Queues{
	CommandPools::CommandPools(Context& context) : 
		singleUseTransfert(context),
		graphics(context){}
}