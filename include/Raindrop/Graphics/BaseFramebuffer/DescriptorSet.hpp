#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_DESCRIPTOR_SET_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_DESCRIPTOR_SET_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	class DescriptorSet{
		public:
			DescriptorSet(Context& context);
			~DescriptorSet();

			VkDescriptorSet set() const;
			VkDescriptorSetLayout layout() const;
			VkDescriptorPool pool() const;

			void update(VkImageView image);

		private:
			Context& _context;
			VkDescriptorSet _set;
			VkDescriptorSetLayout _layout;
			VkDescriptorPool _pool;

			void createPool();
			void createLayout();
			void createSet();
			void destroyPool();
			void destroyLayout();
			void destroySet();
	};
}

#endif