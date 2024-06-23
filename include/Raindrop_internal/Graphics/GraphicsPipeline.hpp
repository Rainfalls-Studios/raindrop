#ifndef __RAINDROP_INTERNAL_GRAPHICS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_GRAPHICS_PIPELINE_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct GraphicsPipelineConfigInfo{
		VkPipelineCreateFlags flags;
		VkPipelineVertexInputStateCreateInfo vertexInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineTessellationStateCreateInfo tessellationInfo;
		std::vector<VkPipelineShaderStageCreateInfo> stages;
		uint32_t subpass;
		void* pNext;

		std::list<std::shared_ptr<Shader>> shaders;
		std::shared_ptr<PipelineLayout> pipelineLayout;
		std::shared_ptr<RenderPass> renderPass;

		static void defaultInfo(GraphicsPipelineConfigInfo& info);
	};
	
	class GraphicsPipeline{
		public:
			GraphicsPipeline(Context& context, const GraphicsPipelineConfigInfo& info);
			~GraphicsPipeline();

			void bind(VkCommandBuffer commandBuffer) noexcept;
			VkPipeline get() const;
			std::shared_ptr<PipelineLayout> getLayout() const;
			std::shared_ptr<RenderPass> getRenderPass() const;

		private:
			Context& _context;
			VkPipeline _pipeline;
			std::shared_ptr<PipelineLayout> _layout;
			std::shared_ptr<RenderPass> _renderPass;
			std::list<std::shared_ptr<Shader>> _shaders;
	};
}

#endif