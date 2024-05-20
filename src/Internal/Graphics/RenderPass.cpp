#include <Raindrop_internal/Graphics/RenderPass.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	RenderPass::RenderPass(Context& context, const RenderPassConfigInfo& config) : 
		_context{context},
		_renderPass{VK_NULL_HANDLE}
	{
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		_context.getLogger()->info("Creating new render pass...");

		VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		std::vector<VkSubpassDescription> subpasses(config.subpasses.size());
		for (std::size_t i=0; i<subpasses.size(); i++){
			auto& src = config.subpasses[i];
			auto& dst = subpasses[i];

			dst.flags = src.description.flags;

			dst.colorAttachmentCount = static_cast<uint32_t>(src.color.size());
			dst.pColorAttachments = src.color.data();

			dst.preserveAttachmentCount = static_cast<uint32_t>(src.preserve.size());
			dst.pPreserveAttachments = src.preserve.data();

			dst.inputAttachmentCount = static_cast<uint32_t>(src.input.size());
			dst.pInputAttachments = src.input.data();

			if (src.depth){
				dst.pDepthStencilAttachment = &src.depth.value();
			} else {
				dst.pDepthStencilAttachment = nullptr;
			}

			dst.pResolveAttachments = nullptr;
		}

		info.subpassCount = static_cast<uint32_t>(subpasses.size());
		info.pSubpasses = subpasses.data();
		
		info.attachmentCount = static_cast<uint32_t>(config.attachments.size());
		info.pAttachments = config.attachments.data();

		info.dependencyCount = static_cast<uint32_t>(config.dependencies.size());
		info.pDependencies = config.dependencies.data();

		Exceptions::checkVkCreation<VkRenderPass>(
			vkCreateRenderPass(device.get(), &info, allocationCallbacks, &_renderPass),
			"Failed to create render pass"
		);


		_context.getLogger()->info("Render pass created successfully !");
	}

	RenderPass::~RenderPass(){
		_context.getLogger()->info("Destroying render pass ...");
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_renderPass != VK_NULL_HANDLE){
			vkDestroyRenderPass(device.get(), _renderPass, allocationCallbacks);
			_renderPass = VK_NULL_HANDLE;
		}
		_context.getLogger()->info("Render pass destroyed successfully !");
	}

	VkRenderPass RenderPass::get() const{
		return _renderPass;
	}
}