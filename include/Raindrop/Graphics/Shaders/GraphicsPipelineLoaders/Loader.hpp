#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_LOADER_LOADER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_LOADER_LOADER_HPP__

#include <Raindrop/Graphics/Shaders/GraphicsPipelineLoaders/common.hpp>

namespace Raindrop::Graphics::Shaders::GraphicsPipelineLoaders{
	class Loader{
		public:
			Loader(Context& context);
			virtual ~Loader() = default;

			const VkPipelineViewportStateCreateInfo& viewportInfo() const;
			const VkPipelineRasterizationStateCreateInfo& rasterizationInfo() const;
			const VkPipelineMultisampleStateCreateInfo& multisampleInfo() const;
			const VkPipelineInputAssemblyStateCreateInfo& inputAssemblyInfo() const;
			const VkPipelineColorBlendStateCreateInfo& colorBlendInfo() const;
			const VkPipelineDepthStencilStateCreateInfo& depthStencilInfo() const;
			const VkPipelineTessellationStateCreateInfo& tessellationInfo() const;
			const VkPipelineColorBlendAttachmentState& attachmentState(std::size_t id) const;

			const std::string& name() const;
			const VkRenderPass& renderPass() const;
			const uint32_t& subpass() const;
		
		protected:
			Context& _context;

			VkRenderPass _renderPass = VK_NULL_HANDLE;
			uint32_t _subpass = 0;

			VkPipelineViewportStateCreateInfo _viewportInfo{};
			VkPipelineRasterizationStateCreateInfo _rasterizationInfo{};
			VkPipelineMultisampleStateCreateInfo _multisampleInfo{};
			VkPipelineInputAssemblyStateCreateInfo _inputAssemblyInfo{};
			VkPipelineColorBlendStateCreateInfo _colorBlendInfo{};
			VkPipelineDepthStencilStateCreateInfo _depthStencilInfo{};
			VkPipelineTessellationStateCreateInfo _tessellationInfo{};

			std::vector<std::shared_ptr<Shader>> _shaders;
			std::vector<VkPipelineColorBlendAttachmentState> _colorAttachments;
			std::vector<VkDescriptorSetLayout> _setLayouts;
			std::vector<VkVertexInputAttributeDescription> _vertexAttributes;
			std::vector<VkVertexInputBindingDescription> _vertexBindings;
			std::vector<VkPushConstantRange> _pushConstants;
			std::string _name;
	};
}

#endif