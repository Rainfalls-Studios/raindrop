#include <Raindrop/Renderer/Queues/CommandPools/Graphics.hpp>
#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer::Queues{
	Graphics::Graphics(Context& context) : CommandPool(context){
		CommandPool::create();
	}

	VkCommandPoolCreateInfo Graphics::createInfo() const{
		return VkCommandPoolCreateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = static_cast<uint32_t>(_context.queues.graphicsFamily())
		};
	}
}