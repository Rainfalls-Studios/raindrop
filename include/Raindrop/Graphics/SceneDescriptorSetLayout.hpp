#ifndef __RAINDROP_GRAPHICS_SCENE_DESCRIPTOR_SET_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_SCENE_DESCRIPTOR_SET_LAYOUT_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class SceneDescriptorSetLayout{
		public:
			SceneDescriptorSetLayout(GraphicsContext& context);
			~SceneDescriptorSetLayout();

			VkDescriptorSetLayout get() const;

		private:
			GraphicsContext& _context;
			VkDescriptorSetLayout _setLayout;

			void createSetLayout();
			void destroySetLayout();

	};
}

#endif