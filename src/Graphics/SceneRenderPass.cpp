#include <Raindrop/Graphics/SceneRenderPass.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

#include "configurations/Scene.hpp"

namespace Raindrop::Graphics{
	SceneRenderPass::SceneRenderPass(GraphicsContext& context) : _context{context}{
		createRenderPass();
	}

	SceneRenderPass::~SceneRenderPass(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (_renderPass) vkDestroyRenderPass(device, _renderPass, allocationCallbacks);
		_renderPass = VK_NULL_HANDLE;
	}

	void SceneRenderPass::createRenderPass(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		
		std::vector<VkAttachmentReference> colorAttachmentRefs(attachments.size() - 1);
		for (int i=1; i<attachments.size(); i++){
			auto& ref = colorAttachmentRefs[i-1];
			ref.attachment = i;
			ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size());
		subpass.pColorAttachments = colorAttachmentRefs.data();
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> descriptions(attachments.size());
		for (int i=0; i<attachments.size(); i++){
			descriptions[i] = attachments[i].description;
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(descriptions.size());
		renderPassInfo.pAttachments = descriptions.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(device, &renderPassInfo, allocationCallbacks, &_renderPass) != VK_SUCCESS){
			throw std::runtime_error("Failed to create scene render pass");
		}
	}

	VkRenderPass SceneRenderPass::get() const{
		return _renderPass;
	}
}