#ifndef __RAINDROP_RENDERER_MATERIAL_SET_LAYOUT_HPP__
#define __RAINDROP_RENDERER_MATERIAL_SET_LAYOUT_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Material{
	class SetLayout{
		public:
			SetLayout(Context& context);
			~SetLayout();

			VkDescriptorSetLayout get();

		private:
			Context& _context;
			VkDescriptorSetLayout _setLayout;

			void createSetLayout();

	};
}

#endif