#include <Raindrop/Graphics/Internal/Queue.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>
#include <Raindrop/Graphics/Internal/QueueFamily.hpp>
#include <Raindrop/Graphics/Internal/QueueHandler.hpp>
#include <Raindrop/Graphics/Internal/CommandBuffer.hpp>

namespace Raindrop::Graphics::Internal{
	Queue::Queue(QueueFamily& family, std::size_t queueIndex) : 
		_family{family},
		_index{queueIndex}{

		auto device = _family._queueHandler._context.device.get();
		vkGetDeviceQueue(device, family.index(), queueIndex, &_queue);
	}
		
	Queue::~Queue(){}

	VkQueue Queue::get() const{
		return _queue;
	}

	QueueFamily& Queue::family(){
		return _family;
	}

	const QueueFamily& Queue::family() const{
		return _family;
	}

	std::size_t Queue::index() const{
		return _index;
	}

	VkResult Queue::waitIdle() const{
		return vkQueueWaitIdle(_queue);
	}

	VkResult Queue::submit(uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence){
		return vkQueueSubmit(_queue, submitCount, pSubmits, fence);
	}

	VkResult Queue::present(const VkPresentInfoKHR* info){
		return vkQueuePresentKHR(_queue, info);
	}

	void Queue::submit(CommandBuffer& commandBuffer){
		VkCommandBuffer cmdBuffer = commandBuffer.get();

		VkSubmitInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &cmdBuffer;
		info.signalSemaphoreCount = 0;
		info.waitSemaphoreCount = 0;
		
		if (submit(1, &info, VK_NULL_HANDLE) != VK_SUCCESS){
			_family._queueHandler._context.logger.error("Failed to submit a command buffer");
			throw std::runtime_error("Failed to submit command buffer");
		}
	}

	bool Queue::operator==(const Queue& other) const{
		return _queue == other._queue; 
	}
}