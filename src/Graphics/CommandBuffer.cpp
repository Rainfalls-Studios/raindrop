#include <Raindrop/Graphics/CommandBuffer.hpp>
#include <Raindrop/Graphics/Context.hpp>

namespace Raindrop::Graphics{
	CommandBuffer::CommandBuffer() : 
		_context{nullptr},
		_commandBuffer{VK_NULL_HANDLE}
	{}

	CommandBuffer::CommandBuffer(Context& context, VkCommandBuffer commandBuffer) : 
		_context{&context},
		_commandBuffer{commandBuffer}
	{}


	CommandBuffer::~CommandBuffer(){

	}

	const VkCommandBuffer& CommandBuffer::get() const noexcept{
		return _commandBuffer;
	}
}