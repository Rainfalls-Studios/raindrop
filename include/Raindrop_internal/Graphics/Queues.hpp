#ifndef __RAINDROP_INTERNAL_GRAPHICS_QUEUES_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_QUEUES_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{

	class Queue{
		public:
			Queue(Context& context, const VkQueue& queue);
			~Queue();

			VkQueue get() const;

			void waitIdle();

		private:
			Context* _context;
			VkQueue _queue;
	};

	class QueueFamily{
		public:
			QueueFamily(Context& context, const uint32_t& index, const VkQueueFamilyProperties& properties);
			~QueueFamily();

			uint32_t getIndex() const;
			uint32_t getQueueCount() const;
			VkQueueFlags getQueueFlags() const;
			bool isPresentSupported(VkSurfaceKHR surface) const;

			const std::vector<Queue>& getQueues() const;
			const Queue& getQueue(const std::size_t& id) const;
			Queue& getQueue(const std::size_t& id);

		private:
			Context& _context;
			uint32_t _index;
			VkQueueFamilyProperties _properties;
			std::vector<Queue> _queues;
	};

	class Queues{
		public:
			Queues(Context& context);
			~Queues();

			const std::vector<QueueFamily>& getFamilies() const;

			std::list<std::pair<QueueFamily&, int>> findSuitable(const VkQueueFlags& requiredProperties, const VkSurfaceKHR& surface = VK_NULL_HANDLE);

		private:
			Context& _context;
			std::vector<QueueFamily> _families;
	};
}

#endif