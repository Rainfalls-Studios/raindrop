#ifndef __RAINDROP_GRAPHICS_SYNC_QUEUE_FAMILY_HPP__
#define __RAINDROP_GRAPHICS_SYNC_QUEUE_FAMILY_HPP__

#include "Queue.hpp"
#include "prototypes.hpp"
#include <Raindrop/Graphics/pch.pch>
#include <Raindrop/Graphics/Core/prototypes.hpp>

namespace Raindrop::Graphics::Sync{
	class QueueFamily{
		public:
			static constexpr std::size_t INVALID_INDEX = std::size_t(~0ULL);

			QueueFamily();
			~QueueFamily();

			void initialize(Context& context, Core::Context& core, const std::size_t& familyIndex);
			void release();

			const std::size_t& getIndex() const noexcept;
			const std::size_t& getQueueCount() const noexcept;
			const VkQueueFlags& getFlags() const noexcept;
			const std::vector<Queue>& getQueues() const noexcept;
			const Queue& getQueue(const std::size_t& index) const;

			bool isPresentSupported(const VkSurfaceKHR& surface) const;

		private:
			Context* _context;
			Core::Context* _core;
			std::size_t _index;
			std::size_t _queueCount;
			VkQueueFlags _flags;
			std::vector<Queue> _queues;
	};
}

#endif