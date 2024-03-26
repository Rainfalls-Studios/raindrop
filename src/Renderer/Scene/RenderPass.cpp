#include <Raindrop/Renderer/Scene/RenderPass.hpp>
#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer::Scene{
	RenderPass::RenderPass(Context& context) :
		_context{context},
		_renderPass{VK_NULL_HANDLE}{
		
		createRenderPass();
	}

	RenderPass::~RenderPass(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_renderPass != VK_NULL_HANDLE){
			vkDestroyRenderPass(device, _renderPass, allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}
	}

	void RenderPass::createRenderPass(){
		VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.flags = 0;

		VkAttachmentDescription descriptions[2] = {};
		descriptions[0].flags = 0;
		descriptions[0].format = VK_FORMAT_D32_SFLOAT;
		descriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
		descriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		descriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		descriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		descriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		descriptions[0].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		descriptions[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

		descriptions[1].flags = 0;
		descriptions[1].format = VK_FORMAT_R8G8B8A8_SRGB;
		descriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
		descriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		descriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		descriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		descriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		descriptions[1].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		descriptions[1].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		
		info.pAttachments = descriptions;
		info.attachmentCount = static_cast<uint32_t>(sizeof(descriptions) / sizeof(descriptions[0]));

		VkSubpassDependency dependencies[1] = {};
		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[0].srcAccessMask = 0;//VK_ACCESS_2_NONE_KHR | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		info.dependencyCount = static_cast<uint32_t>(sizeof(dependencies) / sizeof(dependencies[0]));
		info.pDependencies = dependencies;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 1;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 0;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		info.subpassCount = 1;
		info.pSubpasses = &subpass;

		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (vkCreateRenderPass(device, &info, allocationCallbacks, &_renderPass) != VK_SUCCESS){
			throw std::runtime_error("Failed to create render pass");
		}
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}
}