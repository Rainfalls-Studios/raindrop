#include <Raindrop/Graphics/ShadowMap/Sun/RenderPass.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::ShadowMap::Sun{
	VkAttachmentDescription description = VkAttachmentDescription{	
		.flags = 0,
		.format = VK_FORMAT_D32_SFLOAT,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
	};

	RenderPass::RenderPass(GraphicsContext& context) :
		_context{context},
		_renderPass{VK_NULL_HANDLE}{

		createRenderPass();
	}

	RenderPass::~RenderPass(){
		if (_renderPass) vkDestroyRenderPass(_context.device().get(), _renderPass, _context.allocationCallbacks);
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}

	void RenderPass::createRenderPass(){
		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 0;
		subpass.pColorAttachments = nullptr;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &description;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(_context.device().get(), &renderPassInfo, _context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ShadowMap.Sun.RenderPass") << "Failed to create sun shadow map render pass";
			throw std::runtime_error("Failed to create sun shadow map render pass");
		}
	}
}