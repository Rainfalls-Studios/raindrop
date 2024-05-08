#include <Raindrop_internal/Graphics/Framebuffer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Graphics/RenderPass.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	Framebuffer::Framebuffer(Context& context, FramebufferConfigInfo& config) : 
		_context{context},
		_framebuffer{VK_NULL_HANDLE}
	{

		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();
		
		_renderPass = config.renderPass;
		_attachments = config.attachments;
		_width = config.width;
		_height = config.height;
		_layers = config.layers;

		assert(_renderPass);
		
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = config.width;
		info.height = config.height;
		info.layers = config.layers;

		info.renderPass = _renderPass->get();

		info.attachmentCount = static_cast<uint32_t>(config.attachments.size());

		std::vector<VkImageView> imageViews(_attachments.size());
		for (std::size_t i=0; i<imageViews.size(); i++){
			imageViews[i] = _attachments[i]->get();
		}

		info.pAttachments = imageViews.data();

		Exceptions::checkVkCreation<VkFramebuffer>(
			vkCreateFramebuffer(device.get(), &info, allocationCallbacks, &_framebuffer),
			"Failed to create framebuffer"
		);
	}

	Framebuffer::~Framebuffer(){
		auto& device = _context.getDevice();
		const auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_framebuffer != VK_NULL_HANDLE){
			vkDestroyFramebuffer(device.get(), _framebuffer, allocationCallbacks);
			_framebuffer = VK_NULL_HANDLE;
		}
	}

	const VkFramebuffer& Framebuffer::get() const{
		return _framebuffer;
	}

	const std::shared_ptr<RenderPass>& Framebuffer::renderPass() const{
		return _renderPass;
	}

	const std::vector<std::shared_ptr<ImageView>>& Framebuffer::attachments() const{
		return _attachments;
	}
}