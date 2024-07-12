#ifndef __RAINDROP_GRAPHICS_SYNC_QUEUE_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_SYNC_QUEUE_MANAGER_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/Core/prototypes.hpp>
#include "QueueFamily.hpp"

namespace Raindrop::Graphics::Sync{
	class QueueManager{
		public:
			QueueManager();
			~QueueManager();

			void initialize(Context& context, Core::Context& core);
			void release();

			void requireSurfaceSupport(const VkSurfaceKHR& surface);

		private:
			Context* _context;
			Core::Context* _core;
			VkSurfaceKHR _surface;

			Queue* _graphicsQueue;
			Queue* _presentQueue;
			Queue* _transfertQueue;

			std::vector<QueueFamily> _families;

			void findQueues();
	};
}

#endif