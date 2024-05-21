#ifndef __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_PIPELINE_LAYOUT_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_PIPELINE_LAYOUT_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct PipelineLayoutConfigInfo{
		std::vector<VkDescriptorSetLayout> setLayouts;
		std::vector<VkPushConstantRange> pushConstants;
		VkPipelineLayoutCreateFlags flags;
	};

	class PipelineLayout{
		public:
			PipelineLayout(Context& context, const PipelineLayoutConfigInfo& info);
			~PipelineLayout();

			PipelineLayout(const PipelineLayout& other) = delete;
			PipelineLayout& operator=(const PipelineLayout& other) = delete;

			VkPipelineLayout get() const noexcept;

		private:
			Context& _context;
			VkPipelineLayout _layout;

			void createLayout(const PipelineLayoutConfigInfo& info);
			void destroyLayout();
	};
}

#endif