#ifndef __RAINDROP_GRAPHICS_DESCRIPTOR_SET_HPP__
#define __RAINDROP_GRAPHICS_DESCRIPTOR_SET_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Object.hpp>

namespace Raindrop::Graphics{
	class DescriptorSet : public Object{
		public:
			DescriptorSet() noexcept;
			DescriptorSet(Context& context, const VkDescriptorSet& set) noexcept;
			~DescriptorSet();

			const VkDescriptorSet& get() const noexcept;
		
		private:
			Context* _context;
			VkDescriptorSet _set;
	};
}

#endif