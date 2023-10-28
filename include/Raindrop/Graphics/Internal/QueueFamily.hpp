#ifndef __RAINDROP_GRAPHICS_INTERNAL_QUEUE_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_QUEUE_FAMILY_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class QueueFamily{
		friend class Queue;
		public:
			QueueFamily(QueueHandler& queueHandler, std::size_t familyIndex, std::size_t queueCount, const QueueProperties& properties);
			~QueueFamily();

			VkQueueFlags flags() const;
			const QueueProperties& properties() const;
			std::size_t index() const;
			bool supportPresent() const;

			Queue& get(std::size_t index);
			const Queue& get(std::size_t index) const;

		private:
			QueueHandler& _queueHandler;
			std::size_t _familyIndex;
			
			QueueProperties _properties;
			std::vector<std::unique_ptr<Queue>> _queues;
	};
}

#endif