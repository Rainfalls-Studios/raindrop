#ifndef __RAINDROP_GRAPHICS_INTERNAL_QUEUES_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_QUEUES_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class Queues{
		public:
			Queues(Context& context);
			~Queues();

			QueueFamily& graphicsFamily();
			QueueFamily& presentFamily();

		private:
			Context& _context;

			QueueFamily* _graphicsFamily;
			QueueFamily* _presentFamily;

			void getFamilies();
	};
}

#endif