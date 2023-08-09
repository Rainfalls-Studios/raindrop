#include <Raindrop/Graphics/ShadowMap/Sun/ShadowMap.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::ShadowMap::Sun{
	struct AttachmentInfo{
		VkImageViewCreateInfo imageView;
		VkImageCreateInfo image;
		VkClearValue clear;
	};

	AttachmentInfo attachment{
		.imageView = VkImageViewCreateInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = VK_NULL_HANDLE, // Set afterward
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = VK_FORMAT_D32_SFLOAT,
			.components = VkComponentMapping{},
			.subresourceRange = VkImageSubresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
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
			.format = VK_FORMAT_D32_SFLOAT,
			.extent = VkExtent3D{0, 0, 1}, // Set afterward
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
			.queueFamilyIndexCount = 0,					// again.
			.pQueueFamilyIndices = nullptr,				// again
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		},

		.clear = VkClearValue{
			.depthStencil = VkClearDepthStencilValue{
				.depth = 1.f,
				.stencil = 0,
			}
		},
	};

	ShadowMap::ShadowMap(GraphicsContext& context, uint32_t width, uint32_t height) :
		_context{context},
		_width{width},
		_height{height}{
			
		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.ShadowMap.ShadowMap");
		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

		CLOG(INFO, "Engine.Graphics.ShadowMap.ShadowMap") << "Creating sun shadow map...";

		createAttachment();
		createFramebuffer();

		CLOG(INFO, "Engine.Graphics.ShadowMap.ShadowMap") << "Sun shadow map created with success !";
	}

	ShadowMap::~ShadowMap(){
		CLOG(INFO, "Engine.Graphics.ShadowMap.ShadowMap") << "Destroying sun shadow map...";
		if (_memory) vkFreeMemory(_context.device.get(), _memory, _context.allocationCallbacks);
		if (_image) vkDestroyImage(_context.device.get(), _image, _context.allocationCallbacks);
		if (_imageView) vkDestroyImageView(_context.device.get(), _imageView, _context.allocationCallbacks);
		if (_sampler) vkDestroySampler(_context.device.get(), _sampler, _context.allocationCallbacks);
		if (_framebuffer) vkDestroyFramebuffer(_context.device.get(), _framebuffer, _context.allocationCallbacks);
		CLOG(INFO, "Engine.Graphics.ShadowMap.ShadowMap") << "Sun shadow map destroyed with success !";
	}

	void ShadowMap::createAttachment(){
		createImage();
		allocateMemory();
		createImageView();
		createSampler();
	}

	void ShadowMap::createFramebuffer(){
		VkFramebufferCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;

		info.width = _width;
		info.height = _height;
		info.layers = 1;

		info.pAttachments = &_imageView;
		info.attachmentCount = 1;
		info.renderPass = _context.renderPasses.sun.get();
		
		if (vkCreateFramebuffer(_context.device.get(), &info, _context.allocationCallbacks, &_framebuffer) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.WorldFramebuffer") << "Failed to create world framebuffer";
			throw std::runtime_error("Failed to create world framebuffer");
		}
	}

	void ShadowMap::createImage(){
		VkImageCreateInfo imageInfo{};

		imageInfo = attachment.image;
		imageInfo.extent.width = _width;
		imageInfo.extent.height = _height;

		uint32_t familyIndices[] = {_context.graphicsFamily};

		imageInfo.pQueueFamilyIndices = familyIndices;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.queueFamilyIndexCount = sizeof(familyIndices) / sizeof(uint32_t);
		
		if (vkCreateImage(_context.device.get(), &imageInfo, _context.allocationCallbacks, &_image) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ShadowMap.ShadowMap") << "Failed to create sun shadow map attachment image";
			throw std::runtime_error("Failed to create sun shadow map attachment image");
		}
	}

	void ShadowMap::allocateMemory(){
		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(_context.device.get(), _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;

		if (vkAllocateMemory(_context.device.get(), &allocationInfo, _context.allocationCallbacks, &_memory) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ShadowMap.ShadowMap") << "Failed to allocate sun shadow map attachment memory";
			throw std::runtime_error("Failed to allocate sun shadow map attachment memory");
		}

		if (vkBindImageMemory(_context.device.get(), _image, _memory, 0) != VK_SUCCESS){
			CLOG(ERROR, "Engine.graphics.ShadowMap.ShadowMap") << "Failed to bind sun shadow map attachment image memory";
			throw std::runtime_error("Failed to bind sun shadow map attachment image memory");
		}
	}

	void ShadowMap::createImageView(){
		VkImageViewCreateInfo imageViewInfo{};
		imageViewInfo = attachment.imageView;
		imageViewInfo.image = _image;

		if (vkCreateImageView(_context.device.get(), &imageViewInfo, _context.allocationCallbacks, &_imageView) != VK_SUCCESS){
			CLOG(ERROR, "Engine.Graphics.ShadowMap.ShadowMap") << "Failed to create sun shadow map attachment image view";
			throw std::runtime_error("Failed to create sun shadow map attachment image view");
		}
	}

	void ShadowMap::createSampler(){
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
			CLOG(ERROR, "Engine.Graphics.ShadowMap.ShadowMap") << "Failed to create sun shadow map attachment sampler";
			throw std::runtime_error("Failed to create sun shadow map attachment sampler");
		}
	}

	void ShadowMap::beginRenderPass(VkCommandBuffer commandBuffer) const{
		VkRenderPassBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.framebuffer = _framebuffer;
		info.renderPass = _context.renderPasses.sun.get();
		info.renderArea = VkRect2D{0, 0, _width, _height};

		info.clearValueCount = 1;
		info.pClearValues = &attachment.clear;

		VkImageMemoryBarrier layoutTransitionBarrier = {};

		layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		layoutTransitionBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		layoutTransitionBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layoutTransitionBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.image = _image;
		layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
		layoutTransitionBarrier.subresourceRange.levelCount = 1;
		layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
		layoutTransitionBarrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			0,
			0,
			nullptr,
			0,
			nullptr,
			1,
			&layoutTransitionBarrier
		);
		
		vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	void ShadowMap::endRenderPass(VkCommandBuffer commandBuffer) const{
		vkCmdEndRenderPass(commandBuffer);
	}

	VkDescriptorImageInfo ShadowMap::getAttachmentInfo() const{
		VkDescriptorImageInfo info = {};
		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		info.imageView = _imageView;
		info.sampler = _sampler;
		return info;
	}

	uint32_t ShadowMap::width() const{
		return _width;
	}

	uint32_t ShadowMap::height() const{
		return _height;
	}
}