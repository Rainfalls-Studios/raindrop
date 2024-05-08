#include <Raindrop_internal/Graphics/Queues/CommandPools/SingleUseTransfert.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

namespace Raindrop::Internal::Graphics::Queues{
	SingleUseTransfer::SingleUseTransfer(Context& context) : CommandPool(context){
		CommandPool::create();
	}

	VkCommandPoolCreateInfo SingleUseTransfer::createInfo() const{
		return VkCommandPoolCreateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = static_cast<uint32_t>(_context.getQueues().transfertFamily())
		};
	}
}