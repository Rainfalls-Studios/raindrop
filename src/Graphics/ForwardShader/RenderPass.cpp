// #include <Raindrop/Graphics/ForwardShader/RenderPass.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>

// namespace Raindrop::Graphics::ForwardShader{
// 	VkAttachmentDescription description = VkAttachmentDescription{	
// 		.flags = 0,
// 		.format = VK_FORMAT_R32G32B32A32_SFLOAT,
// 		.samples = VK_SAMPLE_COUNT_1_BIT,
// 		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 		.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
// 		.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 	};

// 	RenderPass::RenderPass(GraphicsContext& context) :
// 		_context{context},
// 		_renderPass{VK_NULL_HANDLE}{
// 		createRenderPass();
// 	}

// 	RenderPass::~RenderPass(){
// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		if (_renderPass) vkDestroyRenderPass(device, _renderPass, allocationCallbacks);
// 	}

// 	void RenderPass::createRenderPass(){
// 		VkAttachmentReference attachmentRef{};
// 		attachmentRef.attachment = 0;
// 		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

// 		VkSubpassDescription subpass = {};
// 		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
// 		subpass.colorAttachmentCount = 1;
// 		subpass.pColorAttachments = &attachmentRef;
// 		subpass.pDepthStencilAttachment = nullptr;

// 		VkSubpassDependency dependency = {};
// 		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
// 		dependency.srcAccessMask = 0;
// 		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 		dependency.dstSubpass = 0;
// 		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
// 		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

// 		VkRenderPassCreateInfo renderPassInfo = {};
// 		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
// 		renderPassInfo.attachmentCount = 1;
// 		renderPassInfo.pAttachments = &description;
// 		renderPassInfo.subpassCount = 1;
// 		renderPassInfo.pSubpasses = &subpass;
// 		renderPassInfo.dependencyCount = 1;
// 		renderPassInfo.pDependencies = &dependency;

// 		if (vkCreateRenderPass(_context.device().get(), &renderPassInfo, _context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader render pass";
// 			throw std::runtime_error("Failed to create forward shader render pass");
// 		}
// 	}

// 	VkRenderPass RenderPass::get() const{
// 		return _renderPass;
// 	}
// }