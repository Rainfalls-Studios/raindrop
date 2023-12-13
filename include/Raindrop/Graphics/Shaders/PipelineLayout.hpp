#ifndef __RAINDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_PIPELINE_LAYOUT_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class PipelineLayout{
		public:
			PipelineLayout(Context& context, const PipelineLayoutBuilder& builder);
			~PipelineLayout();

			VkPipelineLayout get() const;
		
		private:
			Context& _context;

			VkPipelineLayout _layout;
	};
}

#endif