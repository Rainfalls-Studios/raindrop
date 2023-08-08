#include <Raindrop/Graphics/ForwardShader/Framebuffer.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::ForwardShader{

	struct AttachmentInfo{
		VkAttachmentDescription description;
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;
		VkClearValue clear;
	};

	static AttachmentInfo attachment = {
		.description = VkAttachmentDescription{	
			.flags = 0,
			.format = VK_FORMAT_R32G32B32A32_SFLOAT,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		},

		.imageView = VkImageViewCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE, // Set afterward
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_R32G32B32A32_SFLOAT,
			.components = VkComponentMapping{},
			.subresourceRange = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		},

		.image = VkImageCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = VK_FORMAT_R32G32B32A32_SFLOAT,
			.extent = VkExtent3D{0, 0, 1}, // Set afterward
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
			.queueFamilyIndexCount = 0,					// again.
			.pQueueFamilyIndices = nullptr,				// again
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		},

		.clear = VkClearValue{
			.color = VkClearColorValue{
				.uint32 = {0U, 0U, 0U, 0U},
			}
		}
	};

	Framebuffer::Framebuffer(GraphicsContext& context, uint32_t width, uint32_t height) : 
		_context{context},
		_memory{VK_NULL_HANDLE},
		_image{VK_NULL_HANDLE},
		_imageView{VK_NULL_HANDLE},
		_sampler{VK_NULL_HANDLE},
		_framebuffer{VK_NULL_HANDLE},
		_width{width},
		_height{height}{
		
		createImage();
		allocateMemory();
		bindImageMemory();
		createImageView();
		createSampler();
		createFramebuffer();
	}

	Framebuffer::~Framebuffer(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (_framebuffer) vkDestroyFramebuffer(device, _framebuffer, allocationCallbacks);
		if (_imageView) vkDestroyImageView(device, _imageView, allocationCallbacks);
		if (_image) vkDestroyImage(device, _image, allocationCallbacks);
		if (_memory) vkFreeMemory(device, _memory, allocationCallbacks);
		if (_sampler) vkDestroySampler(device, _sampler, allocationCallbacks);
	}
	
	void Framebuffer::createImage(){
		VkImageCreateInfo imageInfo{};
		imageInfo = attachment.image;
		imageInfo.extent.width = _width;
		imageInfo.extent.height = _height;
		
		uint32_t familyIndices[] = {_context.graphicsFamily};

		imageInfo.pQueueFamilyIndices = familyIndices;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = sizeof(familyIndices) / sizeof(uint32_t);

		if (vkCreateImage(_context.device.get(), &imageInfo, _context.allocationCallbacks, &_image) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment image";
			throw std::runtime_error("Failed to create forward shader attachment image");
		}
	}

	void Framebuffer::allocateMemory(){
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(_context.device.get(), _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;
		

		if (vkAllocateMemory(_context.device.get(), &allocationInfo, _context.allocationCallbacks, &_memory) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to allocate forward shader attachment memory";
			throw std::runtime_error("Failed to allocate forward shader attachment memory");
		}
	}

	void Framebuffer::bindImageMemory(){
		if (vkBindImageMemory(_context.device.get(), _image, _memory, 0) != VK_SUCCESS){
			CLOG(ERROR, "Engine.graphics.ForwardShader") << "Failed to bind forward shader attachment image memory";
			throw std::runtime_error("Failed to bind forward shader attachment image memory");
		}
	}

	void Framebuffer::createImageView(){
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo = attachment.imageView;
		imageViewInfo.image = _image;

		if (vkCreateImageView(_context.device.get(), &imageViewInfo, _context.allocationCallbacks, &_imageView) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment image view";
			throw std::runtime_error("Failed to create forward shader attachment image view");
		}
	}

	void Framebuffer::createSampler(){
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		if (vkCreateSampler(_context.device.get(), &samplerInfo, _context.allocationCallbacks, &_sampler) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader attachment sampler";
			throw std::runtime_error("Failed to create forward shader sampler");
		}
	}

	void Framebuffer::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = _width;
		info.height = _height;
		info.layers = 1;

		info.pAttachments = &_imageView;
		info.attachmentCount = 1;
		info.renderPass = _context.renderPasses.forwardShader.get();
		
		if (vkCreateFramebuffer(_context.device.get(), &info, _context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ForwardShader") << "Failed to create forward shader framebuffer";
			throw std::runtime_error("Failed to create forward shader framebuffer");
		}
	}

	uint32_t Framebuffer::width() const{
		return _width;
	}

	uint32_t Framebuffer::height() const{
		return _height;
	}
	
	void Framebuffer::beginRenderPass(VkCommandBuffer commandBuffer){
		transitionImage(commandBuffer);

		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _context.renderPasses.forwardShader.get();
		info.renderArea = VkRect2D{0, 0, _width, _height};

		info.clearValueCount = 1;
		info.pClearValues = &attachment.clear;

		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Framebuffer::endRenderPass(VkCommandBuffer commandBuffer){
		vkCmdEndRenderPass(commandBuffer);
	}

	void Framebuffer::transitionImage(VkCommandBuffer commandBuffer){
		VkImageMemoryBarrier layoutTransitionBarrier = {};
		layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		layoutTransitionBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layoutTransitionBarrier.newLayout = attachment.description.initialLayout;
		layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.image = _image;
		layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
		layoutTransitionBarrier.subresourceRange.levelCount = 1;
		layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
		layoutTransitionBarrier.subresourceRange.layerCount = 1;
		layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0,
			nullptr,
			0,
			nullptr,
			1,
			&layoutTransitionBarrier
		);
	}

	VkDescriptorImageInfo Framebuffer::attachmentInfo() const{
		VkDescriptorImageInfo info;
		info.sampler = _sampler;
		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		info.imageView = _imageView;
		return info;
	}
}