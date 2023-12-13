#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_BUILDER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class GraphicsPipelineBuilder{
		public:
			GraphicsPipelineBuilder(Context& context);
			~GraphicsPipelineBuilder();

			void loadFromNode(const YAML::Node& node);

			const VkPipelineVertexInputStateCreateInfo& vertexInputStateCreateInfo() const;
			const VkPipelineViewportStateCreateInfo& viewportStateCreateInfo() const;
			const VkPipelineRasterizationStateCreateInfo& rasterizationStateCreateInfo() const;
			const VkPipelineMultisampleStateCreateInfo& multisampleStateCreateInfo() const;
			const VkPipelineInputAssemblyStateCreateInfo& inputAssemblyStateCreateInfo() const;
			const VkPipelineColorBlendStateCreateInfo& colorBlendStateCreateInfo() const;
			const VkPipelineDepthStencilStateCreateInfo& depthStencilStateCreateInfo() const;
			const VkPipelineTessellationStateCreateInfo& tessellationStateCreateInfo() const;
			const VkPipelineShaderStageCreateInfo* stages() const;
			const VkPipelineDynamicStateCreateInfo& dynamicStateCreateInfo() const;
			std::size_t stageCount() const;

			const std::string& name() const;
			const VkPipelineCreateFlags& flags() const;
			const std::shared_ptr<PipelineLayout>& layout() const;
			const std::shared_ptr<RenderPass::RenderPass>& renderPass() const;
			const std::list<std::shared_ptr<Shader>>& shaders() const;
			uint32_t subpass() const;


		
		private:
			Context& _context;

			std::shared_ptr<RenderPass::RenderPass> _renderPass;
			std::shared_ptr<GraphicsPipeline> _base;
			std::shared_ptr<PipelineLayout> _layout;
			uint32_t _subpass;

			VkPipelineVertexInputStateCreateInfo _vertexInputStateCreateInfo;
			VkPipelineInputAssemblyStateCreateInfo _inputAssemblyStateCreateInfo;
			VkPipelineTessellationStateCreateInfo _tessellationStateCreateInfo;
			VkPipelineViewportStateCreateInfo _viewportStateCreateInfo;
			VkPipelineRasterizationStateCreateInfo _rasterizationStateCreateInfo;
			VkPipelineMultisampleStateCreateInfo _multisampleStateCreateInfo;
			VkPipelineDepthStencilStateCreateInfo _depthStencilStateCreateInfo;
			VkPipelineColorBlendStateCreateInfo _colorBlendStateCreateInfo;
			VkPipelineDynamicStateCreateInfo _dynamicStateCreateInfo;
			VkPipelineCreateFlags _flags;

			std::vector<VkPipelineShaderStageCreateInfo> _stages;
			std::vector<VkPipelineColorBlendAttachmentState> _colorAttachments;
			std::vector<VkDescriptorSetLayout> _setLayouts;
			std::vector<VkVertexInputAttributeDescription> _vertexAttributes;
			std::vector<VkVertexInputBindingDescription> _vertexBindings;
			std::vector<VkPushConstantRange> _pushConstants;
			std::vector<VkDynamicState> _dynamicStates;

			std::vector<VkViewport> _viewports;
			std::vector<VkRect2D> _scissors;
			std::list<std::shared_ptr<Shader>> _shaders;
			std::list<std::string> _names;
			std::string _name;
			std::unique_ptr<VkSampleMask[]> _sampleMasks;

			void loadStage(const YAML::Node& node);

			void loadVertexInputStateCreateInfo(const YAML::Node& node);
			void loadInputAssemblyStateCreateInfo(const YAML::Node& node);
			void loadTessellationStateCreateInfo(const YAML::Node& node);
			void loadViewportStateCreateInfo(const YAML::Node& node);
			void loadRasterizationStateCreateInfo(const YAML::Node& node);
			void loadMultisampleStateCreateInfo(const YAML::Node& node);
			void loadDepthStencilStateCreateInfo(const YAML::Node& node);
			void loadColorBlendStateCreateInfo(const YAML::Node& node);
			void loadDynamicStateCreateInfo(const YAML::Node& node);

			void loadVertexAttribute(const YAML::Node& node);
			void loadVertexBinding(const YAML::Node& node);
			void loadViewport(const YAML::Node& node);
			void loadScissor(const YAML::Node& node);
			void loadBlendConstants(const YAML::Node& node);
			void loadColorAttachment(const YAML::Node& node);
		
			void loadStencilOpState(const YAML::Node& node, VkStencilOpState& state);
			VkSpecializationInfo* getSpecializationInfo(const YAML::Node& node);
	};
}

#endif