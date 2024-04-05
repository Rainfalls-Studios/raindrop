#include <Raindrop/Graphics/Pipelines/GraphicsPipeline.hpp>
#include <Raindrop/Graphics/Pipelines/PipelineLayout.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <spdlog/spdlog.h>

namespace Raindrop::Graphics::Pipelines{
	GraphicsPipelineConfigInfo::GraphicsPipelineConfigInfo() : 
		vertexInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO},
		viewportInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO},
		inputAssemblyInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO},
		rasterizationInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO},
		multisampleInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO},
		colorBlendInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO},
		depthStencilInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO},
		dynamicStateInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO},
		tessellationInfo{.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO},
		pipelineLayoutID{INVALID_LAYOUT_ID},
		renderPass{VK_NULL_HANDLE},
		subpass{0},
		flags{0},
		pNext{nullptr}
	{}
	
	void GraphicsPipelineConfigInfo::assertValidity() const noexcept{
		assert(pipelineLayoutID != INVALID_LAYOUT_ID && "The pipeline layout has not been set");
		assert(renderPass != VK_NULL_HANDLE && "The render pass has not been set");
	}

	void GraphicsPipelineConfigInfo::defaultInfo(GraphicsPipelineConfigInfo& info){
		
		info.vertexInfo.vertexAttributeDescriptionCount = 0;
		info.vertexInfo.pVertexAttributeDescriptions = nullptr;
		info.vertexInfo.vertexBindingDescriptionCount = 0;
		info.vertexInfo.pVertexBindingDescriptions = nullptr;

		info.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		info.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		info.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		info.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		info.viewportInfo.viewportCount = 0;
		info.viewportInfo.pViewports = nullptr;
		info.viewportInfo.scissorCount = 0;
		info.viewportInfo.pScissors = nullptr;

		info.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		info.rasterizationInfo.depthClampEnable = VK_FALSE;
		info.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		info.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		info.rasterizationInfo.lineWidth = 1.0f;
		info.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		info.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		info.rasterizationInfo.depthBiasEnable = VK_FALSE;
		info.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		info.rasterizationInfo.depthBiasClamp = 0.0f;
		info.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		info.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		info.multisampleInfo.sampleShadingEnable = VK_FALSE;
		info.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		info.multisampleInfo.minSampleShading = 1.0f;
		info.multisampleInfo.pSampleMask = nullptr;
		info.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		info.multisampleInfo.alphaToOneEnable = VK_FALSE;

		info.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		info.colorBlendInfo.logicOpEnable = VK_FALSE;
		info.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		info.colorBlendInfo.attachmentCount = 0;
		info.colorBlendInfo.pAttachments = nullptr;
		info.colorBlendInfo.blendConstants[0] = 0.0f;
		info.colorBlendInfo.blendConstants[1] = 0.0f;
		info.colorBlendInfo.blendConstants[2] = 0.0f;
		info.colorBlendInfo.blendConstants[3] = 0.0f;

		info.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		info.depthStencilInfo.depthTestEnable = VK_TRUE;
		info.depthStencilInfo.depthWriteEnable = VK_TRUE;
		info.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		info.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		info.depthStencilInfo.minDepthBounds = 0.0f;
		info.depthStencilInfo.maxDepthBounds = 1.0f;
		info.depthStencilInfo.stencilTestEnable = VK_FALSE;
		info.depthStencilInfo.front = {};
		info.depthStencilInfo.back = {};

		info.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		info.dynamicStateInfo.dynamicStateCount = 0;
		info.dynamicStateInfo.pDynamicStates = nullptr;

		info.tessellationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		info.tessellationInfo.patchControlPoints = 1;
	}

	void GraphicsPipelineConfigInfo::update(){
		dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());

		colorBlendInfo.pAttachments = colorAttachments.data();
		colorBlendInfo.attachmentCount = static_cast<uint32_t>(colorAttachments.size());

		vertexInfo.pVertexAttributeDescriptions = vertices.data();
		vertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertices.size());

		vertexInfo.pVertexBindingDescriptions = bindings.data();
		vertexInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
	}

	GraphicsPipeline::GraphicsPipeline(Context& context, const GraphicsPipelineConfigInfo& info) :
			_context{context},
			_pipeline{VK_NULL_HANDLE}{
		info.assertValidity();

		spdlog::info("Constructing graphics pipeline...");
		VkGraphicsPipelineCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.pNext = info.pNext;
		createInfo.flags = info.flags;
		createInfo.stageCount = static_cast<uint32_t>(info.stages.size());
		createInfo.pStages = info.stages.data();
		createInfo.pVertexInputState = &info.vertexInfo;
		createInfo.pInputAssemblyState = &info.inputAssemblyInfo;
		createInfo.pViewportState = &info.viewportInfo;
		createInfo.pRasterizationState = &info.rasterizationInfo;
		createInfo.pMultisampleState = &info.multisampleInfo;
		createInfo.pDepthStencilState = &info.depthStencilInfo;
		createInfo.pColorBlendState = &info.colorBlendInfo;
		createInfo.pDynamicState = &info.dynamicStateInfo;
		createInfo.pTessellationState = &info.tessellationInfo;
		createInfo.layout = _context.pipelineLayoutRegistry.get(info.pipelineLayoutID)->get();
		createInfo.renderPass = info.renderPass;
		createInfo.subpass = info.subpass;

		createInfo.basePipelineIndex = -1;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (vkCreateGraphicsPipelines(device.get(), VK_NULL_HANDLE, 1, &createInfo, allocationCallbacks, &_pipeline) != VK_SUCCESS){
			spdlog::error("Failed to create a graphics pipeline");
			throw std::runtime_error("Failed to create a graphics pipeline");
		}
	}

	GraphicsPipeline::~GraphicsPipeline(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_pipeline != VK_NULL_HANDLE){
			vkDestroyPipeline(device.get(), _pipeline, allocationCallbacks);
			_pipeline = VK_NULL_HANDLE;
		}
	}

	void GraphicsPipeline::bind(VkCommandBuffer commandBuffer) noexcept{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
	}
	
	VkPipeline GraphicsPipeline::get() const{
		return _pipeline;
	}
}