#ifndef __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HANDLER_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_QUEUE_HANDLER_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class QueueHandler{
		friend class QueueFamily;
		friend class Queue;
		public:
			QueueHandler(Context& context);
			~QueueHandler();

			std::list<QueueFamily&> getByProperies(const QueueProperties& properties);

			QueueFamily& get(std::size_t index);
			const QueueFamily& get(std::size_t index) const;

		private:
			Context& _context;

			std::list<std::unique_ptr<QueueFamily>> _families;

			void populateFamilies();
	};
}

#endif