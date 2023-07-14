#include <Raindrop/Graphics/Editor/ViewportFramebuffer.hpp>
#include <Raindrop/Graphics/Editor/EditorContext.hpp>

namespace Raindrop::Graphics::Editor{
	ViewportFramebuffer::ViewportFramebuffer(EditorContext& context, uint32_t width, uint32_t height) : Image(context), _context{context}, _width{width}, _height{height}{
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Dev.ViewportFramebuffer");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		createRenderPass();
		createAttachmentImage();
		allocAttachmentMemory();
		createAttachmentImageView();
		createAttachmentSampler();
		createFramebuffer();

		Image::setTexture(_attachmentSampler, _attachmentImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	ViewportFramebuffer::~ViewportFramebuffer(){
		auto device = _context.context.device.get();
		auto allocationCallbacks = _context.context.allocationCallbacks;

		if (_framebuffer) vkDestroyFramebuffer(device, _framebuffer, allocationCallbacks);
		if (_attachmentSampler) vkDestroySampler(device, _attachmentSampler, allocationCallbacks);
		if (_attachmentImageView) vkDestroyImageView(device, _attachmentImageView, allocationCallbacks);
		if (_attachmentMemory) vkFreeMemory(device, _attachmentMemory, allocationCallbacks);
		if (_attachmentImage) vkDestroyImage(device, _attachmentImage, allocationCallbacks);
		if (_renderPass) vkDestroyRenderPass(device, _renderPass, allocationCallbacks);
	}

	void ViewportFramebuffer::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = _renderPass;
		info.width = _width;
		info.height = _height;
		info.layers = 1;

		info.pAttachments = &_attachmentImageView;
		info.attachmentCount = 1;
		
		if (vkCreateFramebuffer(_context.context.device.get(), &info, _context.context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to create viewport framebuffer";
			throw std::runtime_error("Failed to create framebuffer");
		}
	}

	void ViewportFramebuffer::createRenderPass(){
		VkAttachmentReference attachmentRef{};
		attachmentRef.attachment = 0;
		attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription attachmentDescription{};
		attachmentDescription.format = VK_FORMAT_B8G8R8A8_SRGB;
		attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &attachmentRef;
		subpass.pDepthStencilAttachment = nullptr;

		VkRenderPassCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachmentDescription;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;

		if (vkCreateRenderPass(_context.context.device.get(), &info, _context.context.allocationCallbacks, &_renderPass) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to create viewport render pass";
			throw std::runtime_error("Failed to create render passs");
		}
	}
	
	void ViewportFramebuffer::createAttachmentImage(){
		VkImageCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.imageType = VK_IMAGE_TYPE_2D;
		info.format = VK_FORMAT_B8G8R8A8_SRGB;
		info.extent = VkExtent3D{_width, _height, 1};
		info.mipLevels = 1;
		info.arrayLayers = 1;
		info.samples = VK_SAMPLE_COUNT_1_BIT;
		info.tiling = VK_IMAGE_TILING_OPTIMAL;
		info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		info.queueFamilyIndexCount = 1;
		info.pQueueFamilyIndices = &_context.context.graphicsFamily;
		info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		if (vkCreateImage(_context.context.device.get(), &info, _context.context.allocationCallbacks, &_attachmentImage) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to create viewport attachment image";
			throw std::runtime_error("Failed to create attachment image");
		}
	}

	void ViewportFramebuffer::createAttachmentImageView(){
		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _attachmentImage;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = VK_FORMAT_B8G8R8A8_SRGB;
		info.subresourceRange = VkImageSubresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		};

		if (vkCreateImageView(_context.context.device.get(), &info, _context.context.allocationCallbacks, &_attachmentImageView) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to create viewport attachment image view";
			throw std::runtime_error("Failed to create attachment image view");
		}
	}

	void ViewportFramebuffer::allocAttachmentMemory(){
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(_context.context.device.get(), _attachmentImage, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = _context.context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;
		

		if (vkAllocateMemory(_context.context.device.get(), &allocationInfo, _context.context.allocationCallbacks, &_attachmentMemory) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to allocate viewport framebuffer attachment memory";
			throw std::runtime_error("Failed to allocate viewport framebuffer attachment memory");
		}

		if (vkBindImageMemory(_context.context.device.get(), _attachmentImage, _attachmentMemory, 0) != VK_SUCCESS){
			CLOG(ERROR, "Engine.graphics.Dev.ViewportFramebuffer") << "Failed to bind viewport framebuffer attachment image memory";
			throw std::runtime_error("Failed to bind viewport framebuffer attachment image memory");
		}
	}

	void ViewportFramebuffer::createAttachmentSampler(){
		VkSamplerCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.magFilter = VK_FILTER_LINEAR;
		info.minFilter = VK_FILTER_LINEAR;
		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.mipLodBias = 0.0f;
		info.anisotropyEnable = VK_FALSE;
		info.maxAnisotropy = 1.0f;
		info.compareEnable = VK_FALSE;
		info.compareOp = VK_COMPARE_OP_ALWAYS;
		info.minLod = 0.0f;
		info.maxLod = 0.0f;
		info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		info.unnormalizedCoordinates = VK_FALSE;

		if (vkCreateSampler(_context.context.device.get(), &info, _context.context.allocationCallbacks, &_attachmentSampler) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.Dev.ViewportFramebuffer") << "Failed to create viewport framebuffer attachment sampler";
			throw std::runtime_error("Failed to create sampler");
		}
	}

	bool ViewportFramebuffer::beginRenderPass(VkCommandBuffer commandBuffer){
		if (_width == 0 || _height == 0) return false;

		VkRenderPassBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _renderPass;
		info.renderArea = VkRect2D{0, 0, _width, _height};

		VkClearValue clearColor;
		clearColor.color.int32[0] = 0;
		clearColor.color.int32[1] = 0;
		clearColor.color.int32[2] = 0;
		clearColor.color.int32[3] = 0;

		info.pClearValues = &clearColor;
		info.clearValueCount = 1;

		VkImageMemoryBarrier layoutTransitionBarrier{};

		layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		layoutTransitionBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layoutTransitionBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.image = _attachmentImage;
		layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
		layoutTransitionBarrier.subresourceRange.levelCount = 1;
		layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
		layoutTransitionBarrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &layoutTransitionBarrier);
		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

		return true;
	}

	void ViewportFramebuffer::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}

	uint32_t ViewportFramebuffer::width() const{
		return _width;
	}

	uint32_t ViewportFramebuffer::height() const{
		return _height;
	}

	VkRenderPass ViewportFramebuffer::renderPass() const{
		return _renderPass;
	}
}