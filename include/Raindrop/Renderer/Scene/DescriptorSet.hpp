#ifndef __RAINDROP_RENDERER_SCENE_DESCRIPTOR_SET_HPP__
#define __RAINDROP_RENDERER_SCENE_DESCRIPTOR_SET_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Scene{
	class DescriptorSet{
		public:
			DescriptorSet(Context& context);
			~DescriptorSet();

			VkDescriptorSet set() const;
			VkDescriptorSetLayout layout() const;
			VkDescriptorPool pool() const;

			void update();

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