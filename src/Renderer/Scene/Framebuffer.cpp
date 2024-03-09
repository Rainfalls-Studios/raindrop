#include <Raindrop/Renderer/Scene/Framebuffer.hpp>
#include <Raindrop/Renderer/Context.hpp>

namespace Raindrop::Renderer::Scene{
	Framebuffer::Framebuffer(Context& context) : Framebuffer(context, {1080, 720}){}

	Framebuffer::Framebuffer(Context& context, glm::uvec2 size) : 
		_context{context},
		_framebuffer{VK_NULL_HANDLE},
		_size{size}
	{

		createAttachments();
		createFramebuffer();

	}

	Framebuffer::~Framebuffer(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_framebuffer != VK_NULL_HANDLE){
			vkDestroyFramebuffer(device, _framebuffer, allocationCallbacks);
			_framebuffer = VK_NULL_HANDLE;
		}

		destroyAttachments();
	}

	void Framebuffer::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = static_cast<uint32_t>(_size.x);
		info.height = static_cast<uint32_t>(_size.y);

		// info.attachmentCount = 2;
		// info.pAttachments

		info.renderPass = _context.renderPass.get();
	}

	void Framebuffer::createAttachments(){
		createDepthAttachment();
		createColorAttachment();
	}

	void Framebuffer::destroyAttachments(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		if (_depthAttachment.image != VK_NULL_HANDLE){
			vkDestroyImage(device, _depthAttachment.image, allocationCallbacks);
			_depthAttachment.image = VK_NULL_HANDLE;
		}

		if (_colorAttachment.image != VK_NULL_HANDLE){
			vkDestroyImage(device, _colorAttachment.image, allocationCallbacks);
			_colorAttachment.image = VK_NULL_HANDLE;
		}
	}

	VkImageCreateInfo Framebuffer::depthAttachmentInfo(){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = VK_FORMAT_D32_SFLOAT;

		info.extent.width = static_cast<uint32_t>(_size.x);
		info.extent.height = static_cast<uint32_t>(_size.y);
		info.extent.depth = 1;
		info.arrayLayers = 1;

		info.samples = VK_SAMPLE_COUNT_1_BIT;

		info.mipLevels = 1;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return info;
	}

	VkImageCreateInfo Framebuffer::colorAttachmentInfo(){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = VK_FORMAT_R8G8B8A8_SRGB;

		info.extent.width = static_cast<uint32_t>(_size.x);
		info.extent.height = static_cast<uint32_t>(_size.y);
		info.extent.depth = 1;
		info.arrayLayers = 1;

		info.samples = VK_SAMPLE_COUNT_1_BIT;

		info.mipLevels = 1;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return info;
	}

	void Framebuffer::createDepthAttachment(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkImageCreateInfo info = depthAttachmentInfo();
		if (vkCreateImage(device, &info, allocationCallbacks, &_depthAttachment.image) != VK_SUCCESS){
			throw std::runtime_error("Failed to create depth attachment");
		}
	}

	void Framebuffer::createColorAttachment(){
		const auto& device = _context.renderer.device.get();
		const auto& allocationCallbacks = _context.renderer.allocationCallbacks;

		VkImageCreateInfo info = colorAttachmentInfo();
		if (vkCreateImage(device, &info, allocationCallbacks, &_colorAttachment.image) != VK_SUCCESS){
			throw std::runtime_error("Failed to create depth attachment");
		}
	}
}