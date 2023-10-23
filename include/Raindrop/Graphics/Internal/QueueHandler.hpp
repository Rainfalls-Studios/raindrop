#ifndef __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HANDLER_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HANDLER_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class QueueHandler{
		public:
			struct QueueProperties{
				bool present;
				VkQueueFlags flags;

				QueueProperties(VkQueueFlags flags, bool present = false);
				QueueProperties& operator=(VkQueueFlags flags);

				bool operator==(const QueueProperties& other) const;
				bool operator!=(const QueueProperties& other) const;

				static std::size_t hash(const QueueProperties& properties);
			};

			QueueHandler(Context& context);
			~QueueHandler();

			VkQueue get(const QueueProperties& properties);

		private:
			struct QueueData{
				VkQueue queue;
				uint32_t familyIndex;
				uint32_t queueIndex;
			};

			struct FamilyInfo;

			Context& _context;
			std::unordered_map<QueueProperties, QueueData, decltype(QueueProperties::hash)> _queues;
			std::vector<uint32_t> _remainingQueueCount;

			VkQueue query(const QueueProperties& properties);
			void testFamiliesSuitability(const QueueProperties& properties, std::list<FamilyInfo>& families);
	};
}

#endif