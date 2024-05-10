#ifndef __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics::Pipelines{
	struct GraphicsPipelineConfigInfo{
		GraphicsPipelineConfigInfo();

		GraphicsPipelineConfigInfo(const GraphicsPipelineConfigInfo&) = delete;
		GraphicsPipelineConfigInfo &operator=(const GraphicsPipelineConfigInfo&) = delete;

		VkPipelineVertexInputStateCreateInfo vertexInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineTessellationStateCreateInfo tessellationInfo;
		std::shared_ptr<PipelineLayout> pipelineLayout;
		VkRenderPass renderPass;
		uint32_t subpass;

		VkPipelineCreateFlags flags;
		void* pNext;

		std::vector<VkPipelineShaderStageCreateInfo> stages;
		std::vector<VkDynamicState> dynamicStateEnables;
		std::vector<VkPipelineColorBlendAttachmentState> colorAttachments;
		std::vector<VkVertexInputAttributeDescription> vertices;
		std::vector<VkVertexInputBindingDescription> bindings;

		void update();
		void assertValidity() const noexcept;

		static void defaultInfo(GraphicsPipelineConfigInfo& info);
	};
	
	class GraphicsPipeline{
		public:
			GraphicsPipeline(Context& context, const GraphicsPipelineConfigInfo& info);
			~GraphicsPipeline();

			void bind(VkCommandBuffer commandBuffer) noexcept;
			VkPipeline get() const;
			std::shared_ptr<PipelineLayout> getLayout() const;

		private:
			Context& _context;
			VkPipeline _pipeline;
			std::shared_ptr<PipelineLayout> _layout;
	};
}

#endif