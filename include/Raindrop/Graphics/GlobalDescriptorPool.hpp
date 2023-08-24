#ifndef __RAINDROP_GRAPHICS_GLOBAL_DESCRIPTOR_POOL_HPP__
#define __RAINDROP_GRAPHICS_GLOBAL_DESCRIPTOR_POOL_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GlobalDescriptorPool{
		public:
			GlobalDescriptorPool(GraphicsContext& context);
			~GlobalDescriptorPool();

			VkDescriptorPool get() const;

		private:
			GraphicsContext& _context;
			VkDescriptorPool _pool;

			void createPool();
			void destroyPool();

	};
}

#endif