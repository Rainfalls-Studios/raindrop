#ifndef __RAINDROP_RENDERER_MATERIAL_POOL_HPP__
#define __RAINDROP_RENDERER_MATERIAL_POOL_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Material{
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