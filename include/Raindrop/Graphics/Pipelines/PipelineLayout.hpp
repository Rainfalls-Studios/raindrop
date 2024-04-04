#ifndef __RAINDROP_GRAPHICS_PIPELINES_PIPELINE_LAYOUT_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_PIPELINE_LAYOUT_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::Pipelines{
	struct PipelineLayoutConfigInfo{
		std::vector<VkDescriptorSetLayout> setLayouts;
		std::vector<VkPushConstantRange> pushConstants;
	};

	class PipelineLayout{
		public:
			PipelineLayout(Context& context, const PipelineLayoutConfigInfo& info);
			~PipelineLayout();

			PipelineLayout(const PipelineLayout& other) = delete;
			PipelineLayout& operator=(const PipelineLayout& other) = delete;

		private:
			Context& _context;
			VkPipelineLayout _layout;

			void createLayout(const PipelineLayoutConfigInfo& info);
			void destroyLayout();
	};
}

#endif