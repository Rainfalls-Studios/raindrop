#ifndef __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class Queue{
		public:
			Queue(QueueFamily& family, std::size_t index);
			~Queue();

			bool operator==(const Queue& other) const;

			VkQueue get() const;
			QueueFamily& family();
			const QueueFamily& family() const;
			std::size_t index() const;

			VkResult waitIdle() const;
			VkResult submit(uint32_t submitCount, const VkSubmitInfo *pSubmits, VkFence fence);
			VkResult present(const VkPresentInfoKHR* info);

			void submit(CommandBuffer& commandBuffer);

		private:
			QueueFamily& _family;
			VkQueue _queue;
			std::size_t _index;
	};
}

#endif