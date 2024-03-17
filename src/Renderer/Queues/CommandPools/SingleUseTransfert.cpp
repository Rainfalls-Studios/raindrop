#include <Raindrop/Renderer/Queues/CommandPools/SingleUseTransfert.hpp>
#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer::Queues{
	SingleUseTransfer::SingleUseTransfer(Context& context) : CommandPool(context){
		CommandPool::create();
	}

	VkCommandPoolCreateInfo SingleUseTransfer::createInfo() const{
		return VkCommandPoolCreateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = static_cast<uint32_t>(_context.queues.transfertFamily())
		};
	}
}