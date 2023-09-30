#include <Raindrop/Graphics/Framebuffer.hpp>
#include <Raindrop/Graphics/Builders/FramebufferBuilder.hpp>
#include <Raindrop/Graphics/RenderPass.hpp>
#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	Framebuffer::Framebuffer(GraphicsContext& context, const Builders::FramebufferBuilder& builder) : _context{context}{
		_attachments.resize(builder.attachmentCount());
		_framebuffer = VK_NULL_HANDLE;

		for (int i=0; i<_attachments.size(); i++){
			_attachments[i] = builder.attachment(i);
		}

		_renderPass = builder.renderPass();
		_width = builder.width();
		_height = builder.height();
		_layers = builder.layers();

		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = _renderPass->get();
		info.width = _width;
		info.height = _height;
		info.layers = _layers;
		
		std::vector<VkImageView> imageViews(_attachments.size());
		for (int i=0; i<imageViews.size(); i++){
			imageViews[i] = _attachments[i]->get();
		}

		info.pAttachments = imageViews.data();
		info.attachmentCount = imageViews.size();

		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (vkCreateFramebuffer(device, &info, allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			throw std::runtime_error("failed to create framebuffer");
		}
	}

	Framebuffer::~Framebuffer(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (_framebuffer) vkDestroyFramebuffer(device, _framebuffer, allocationCallbacks);
		_framebuffer = VK_NULL_HANDLE;
	}

	VkFramebuffer Framebuffer::get() const{
		return _framebuffer;
	}

	uint32_t Framebuffer::width() const{
		return _width;
	}

	uint32_t Framebuffer::height() const{
		return _height;
	}

	uint32_t Framebuffer::layers() const{
		return _layers;
	}

	std::shared_ptr<RenderPass> Framebuffer::renderPass() const{
		return _renderPass;
	}
}