#include <Raindrop/Graphics/Queues/GraphicsCommandBuffers.hpp>
#include <Raindrop/Graphics/Queues/GraphicsContext.hpp>

namespace Raindrop::Graphics::Queues{
	GraphicsCommandBuffers::GraphicsCommandBuffers(GraphicsContext& context, uint32_t frameCount) : _context{context}{
		updateFrameCount(frameCount);
	}

	GraphicsCommandBuffers::~GraphicsCommandBuffers(){
		freeMainCommandBuffers();
	}

	VkCommandBuffer GraphicsCommandBuffers::getCommandBuffer(uint32_t frame){
		return _mainCommandBuffers[frame];
	}

	void GraphicsCommandBuffers::freeMainCommandBuffers(){
		auto device = _context.device.get();
		auto pool = _context.graphics.commandPool.primary();

		if (!_mainCommandBuffers.empty())
			vkFreeCommandBuffers(device, pool, _mainCommandBuffers.size(), _mainCommandBuffers.data());
	}

	void GraphicsCommandBuffers::freeSecondaryCommandBuffer(){
		auto device = _context.device.get();
		auto pool = _context.graphics.commandPool.primary();

		for (auto cmd : _secondaryCommandBuffers){
			vkFreeCommandBuffers(device, pool, 1, cmd.get());
		}

		_secondaryCommandBuffers.clear();
	}

	void GraphicsCommandBuffers::updateFrameCount(uint32_t frameCount){
		auto device = _context.device.get();
		auto pool = _context.graphics.commandPool.primary();

		freeMainCommandBuffers();

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = frameCount;
		info.commandPool = pool;

		_mainCommandBuffers.resize(frameCount);
		if (vkAllocateCommandBuffers(device, &info, _mainCommandBuffers.data()) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate main command buffers");
		}
	}
	
	std::weak_ptr<VkCommandBuffer> GraphicsCommandBuffers::createSecondary(){
		auto device = _context.device.get();
		auto pool = _context.graphics.commandPool.primary();

		std::shared_ptr<VkCommandBuffer> cmd = std::make_shared<VkCommandBuffer>();

		VkCommandBufferAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		info.commandBufferCount = 1;
		info.commandPool = pool;

		if (vkAllocateCommandBuffers(device, &info, cmd.get()) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate secondary command buffer");
		}

		_secondaryCommandBuffers.push_back(cmd);
		return cmd;
	}

	void GraphicsCommandBuffers::freeSecondary(const std::weak_ptr<VkCommandBuffer>& commandBuffer){
		auto device = _context.device.get();
		auto pool = _context.graphics.commandPool.primary();

		auto cmd = commandBuffer.lock();
		if (cmd){
			_secondaryCommandBuffers.remove(cmd);
			vkFreeCommandBuffers(device, pool, 1, cmd.get());
		}
	}
}