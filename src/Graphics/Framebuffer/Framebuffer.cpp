#include <Raindrop/Graphics/Framebuffer/Framebuffer.hpp>
#include <Raindrop/Graphics/Framebuffer/FramebufferBuilder.hpp>
#include <Raindrop/Graphics/Framebuffer/Context.hpp>
#include <Raindrop/Graphics/Image/ImageView.hpp>
#include <Raindrop/Graphics/RenderPass/RenderPass.hpp>

namespace Raindrop::Graphics::Framebuffer{
	Framebuffer::Framebuffer(Context& context, FramebufferBuilder& builder) : _context{context}{
		_framebuffer = VK_NULL_HANDLE;
		
		_width = builder.width();
		_height = builder.height();
		_layers = builder.layers();

		_renderPass = builder.renderPass();
		_attachments = builder.attachments();

		std::vector<VkImageView> imageView(_attachments.size());
		for (std::size_t i=0; i<imageView.size(); i++){
			imageView[i] = _attachments[i]->get();
		}

		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.flags = builder.flags();
		info.attachmentCount = static_cast<uint32_t>(imageView.size());
		info.pAttachments = imageView.data();
		info.renderPass = _renderPass->get();
		info.width = _width;
		info.height = _height;
		info.layers = _layers;

		const auto& device = _context.graphics().internal().device();
		const auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (vkCreateFramebuffer(device.get(), &info, allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			_context.logger().error("Failed to create framebuffer");
			throw std::runtime_error("Failed to create framebuffer");
		}
	}

	Framebuffer::~Framebuffer(){
		const auto& device = _context.graphics().internal().device();
		const auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_framebuffer != VK_NULL_HANDLE){
			vkDestroyFramebuffer(device.get(), _framebuffer, allocationCallbacks);
			_framebuffer = VK_NULL_HANDLE;
		}
	}
}