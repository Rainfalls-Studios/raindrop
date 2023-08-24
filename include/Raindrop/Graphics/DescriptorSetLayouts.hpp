#ifndef __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUTS_HPP__
#define __RAINDROP_GRAPHICS_DESCRIPTOR_SET_LAYOUTS_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/SceneDescriptorSetLayout.hpp>

namespace Raindrop::Graphics{
	struct DescriptorLayouts{
		VkDescriptorSetLayout sunShadowMapLayout;
		VkDescriptorSetLayout lightPointShadowMapLayout;
		VkDescriptorSetLayout spotlightShadowMapLayout;
		SceneDescriptorSetLayout scene;

		DescriptorLayouts(GraphicsContext& context);
		~DescriptorLayouts();

		private:
			GraphicsContext& _context;

			void createLightsLayouts();
			void destroyLightsLayouts();
	};
}

#endif