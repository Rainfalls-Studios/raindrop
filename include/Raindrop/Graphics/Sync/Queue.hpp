#ifndef __RAINDROP_GRAPHICS_SYNC_QUEUE_HPP__
#define __RAINDROP_GRAPHICS_SYNC_QUEUE_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/pch.pch>
#include <Raindrop/Graphics/Core/prototypes.hpp>

namespace Raindrop::Graphics::Sync{
	class Queue{
		public:
			static constexpr std::size_t INVALID_INDEX = std::size_t(~0ULL);

			Queue();
			~Queue();

			void initialize(Context& context, Core::Context& core, QueueFamily& family, const std::size_t& queueIndex);
			void release();

			const VkQueue& get() const noexcept;
			const QueueFamily& getFamily() const noexcept;
			const std::size_t& getIndex() const noexcept;

		private:
			Context* _context;
			Core::Context* _core;
			QueueFamily* _family;
			VkQueue _queue;
			std::size_t _index;
	};
}

#endif