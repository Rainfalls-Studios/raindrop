#include <Raindrop/Graphics/Shaders/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Shaders/Context.hpp>
#include <Raindrop/Graphics/Shaders/GraphicsPipelineBuilder.hpp>
#include <Raindrop/Graphics/Shaders/PipelineLayout.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>

namespace Raindrop::Graphics::Shaders{
	GraphicsPipeline::GraphicsPipeline(Context& context, const GraphicsPipelineBuilder& builder) : _context{context}{
		VkGraphicsPipelineCreateInfo info{};

		_renderPass = builder.renderPass();
		_layout = builder.layout();

		if (!_renderPass){
			_context.logger().error("Cannot create graphics pipeline \"{}\" without a valid render pass", builder.name());
			throw std::invalid_argument("invalid render pass");
		}

		if (!_layout){
			_context.logger().error("cannot create graphics pipeline \"{}\" without a valid pipeline layout", builder.name());
			throw std::invalid_argument("invalid pipeline layout");
		}

		info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		info.pNext = nullptr;
		info.flags = builder.flags();
		info.stageCount = static_cast<uint32_t>(builder.stageCount());
		info.pStages = builder.stages();
		info.pVertexInputState = &builder.vertexInputStateCreateInfo();
		info.pInputAssemblyState = &builder.inputAssemblyStateCreateInfo();
		info.pTessellationState = &builder.tessellationStateCreateInfo();
		info.pViewportState = &builder.viewportStateCreateInfo();
		info.pRasterizationState = &builder.rasterizationStateCreateInfo();
		info.pMultisampleState = &builder.multisampleStateCreateInfo();
		info.pDepthStencilState = &builder.depthStencilStateCreateInfo();
		info.pColorBlendState = &builder.colorBlendStateCreateInfo();
		info.pDynamicState = &builder.dynamicStateCreateInfo();
		info.layout = _layout->get();
		info.renderPass = _renderPass->get();
		info.subpass = builder.subpass();
		info.basePipelineIndex = 0;
		info.basePipelineHandle = VK_NULL_HANDLE;

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (vkCreateGraphicsPipelines(device.get(), VK_NULL_HANDLE, 1, &info, allocationCallbacks, &_pipeline) != VK_SUCCESS){
			_context.logger().error("Failed to create a graphics pipeline");
			throw std::runtime_error("Failed to create a graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_pipeline) vkDestroyPipeline(device.get(), _pipeline, allocationCallbacks);
	}

	void GraphicsPipeline::bind(VkCommandBuffer commandBuffer){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	}
	
	VkPipeline GraphicsPipeline::get() const{
		return _pipeline;
	}
	
	PipelineLayout& GraphicsPipeline::lauout(){
		return *_layout;
	}

	const PipelineLayout& GraphicsPipeline::layout() const{
		return *_layout;
	}
}