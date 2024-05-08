#ifndef __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_POOL_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_MATERIAL_POOL_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics::Materials{
	class Pool{
		public:
			Pool(Context& context);
			~Pool();

			VkDescriptorPool get() const;
			VkDescriptorSet getSet(const std::size_t& index) const;

		private:
			Context& _context;
			VkDescriptorPool _pool;
			std::vector<VkDescriptorSet> _descriptorSets;

			void createPool();
			void allocateDescriptors();
	};
}

#endif