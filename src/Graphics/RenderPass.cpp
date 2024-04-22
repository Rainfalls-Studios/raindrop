#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	RenderPass::RenderPass(Context& context, const RenderPassConfigInfo& config) : 
		_context{context},
		_renderPass{VK_NULL_HANDLE}
	{
		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;


		VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		info.subpassCount = static_cast<uint32_t>(config.subpasses.size());
		info.pSubpasses = config.subpasses.data();
		
		info.attachmentCount = static_cast<uint32_t>(config.attachments.size());
		info.pAttachments = config.attachments.data();

		info.dependencyCount = static_cast<uint32_t>(config.dependencies.size());
		info.pDependencies = config.dependencies.data();

		Exceptions::checkVkCreation<VkRenderPass>(
			vkCreateRenderPass(device.get(), &info, allocationCallbacks, &_renderPass),
			"Failed to create render pass"
		);
	}

	RenderPass::~RenderPass(){
		auto& device = _context.device;
		const auto& allocationCallbacks = _context.allocationCallbacks;

		if (_renderPass != VK_NULL_HANDLE){
			vkDestroyRenderPass(device.get(), _renderPass, allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}
}