#include <Raindrop/Graphics/Internal/CommandBuffer.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/CommandPool.hpp>

namespace Raindrop::Graphics::Internal{
	CommandBuffer::CommandBuffer(CommandPool& pool, VkCommandBuffer commandBuffer, VkCommandBufferLevel level) : 
		_pool{pool},
		_commandBuffer{commandBuffer},
		_level{level},
		_recording{false}
	{}

	CommandBuffer::~CommandBuffer(){
		_pool.free(*this);
	}

	VkCommandBuffer CommandBuffer::get() const{
		return _commandBuffer;
	}

	bool CommandBuffer::operator==(const CommandBuffer& other) const{
		return _commandBuffer == other._commandBuffer;
	}

	void CommandBuffer::begin(VkCommandBufferUsageFlags usage){
		VkCommandBufferBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.pInheritanceInfo = nullptr;
		info.flags = usage;

		if (vkBeginCommandBuffer(_commandBuffer, &info) != VK_SUCCESS){
			_pool._context.logger.error("Failed to submit a command buffer");
			throw std::runtime_error("Failed to begin command buffer");
		}

		_recording = true;
	}

	void CommandBuffer::end(){
		vkEndCommandBuffer(_commandBuffer);
		_recording = false;
	}

	bool CommandBuffer::recording(){
		return _recording;
	}
}