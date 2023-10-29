// #include <Raindrop/Graphics/Texture.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>
// #include <Raindrop/Graphics/Buffer.hpp>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

// namespace Raindrop::Graphics{
// 	Texture::Texture(GraphicsContext& context, const std::filesystem::path &path) : _context{context}{
// 		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Texture");
// 		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

// 		CLOG(INFO, "Engine.Graphics.Texture") << "Loading texture : " << path << "...";
// 		createImage(path);
// 		createImageView();
// 		createSampler();
// 		CLOG(INFO, "Engine.Graphics.Texture") << "Texture loaded with success !";
// 	}

// 	Texture::Texture(GraphicsContext& context, void* data, uint32_t width, uint32_t height, int channels) : _context{context}{
// 		el::Logger* customLogger = el::Loggers::getLogger("Engine.Graphics.Texture");
// 		customLogger->configurations()->set(el::Level::Global, el::ConfigurationType::Format, "%datetime %level [%logger]: %msg");

// 		CLOG(INFO, "Engine.Graphics.Texture") << "Creating texture from custom data ...";
// 		createImageFromPtr(data, width, height, channels);
// 		createImageView();
// 		createSampler();
// 		CLOG(INFO, "Engine.Graphics.Texture") << "Texture loaded with success !";

// 	}

// 	Texture::Texture(GraphicsContext& context) : _context{context}{}

// 	Texture::~Texture(){
// 		CLOG(INFO, "Engine.Graphics.Texture") << "Destroying texture...";
// 		if (_image) vkDestroyImage(_context.device().get(), _image, _context.allocationCallbacks);
// 		if (_memory) vkFreeMemory(_context.device().get(), _memory, _context.allocationCallbacks);
// 		if (_imageView) vkDestroyImageView(_context.device().get(), _imageView, _context.allocationCallbacks);
// 		if (_sampler) vkDestroySampler(_context.device().get(), _sampler, _context.allocationCallbacks);
// 		CLOG(INFO, "Engine.Graphics.Texture") << "Texture destroyed with success !";
// 	}

// 	void Texture::createImage(const std::filesystem::path &filepath){

// 		int texWidth, texHeight, texChannels;

// 		stbi_uc *pixels = stbi_load(filepath.string().c_str(), &texWidth, &texHeight, &texChannels, 4);

// 		if (!pixels){
// 			CLOG(ERROR, "Engine.Graphics.Texture") << "Failed to load " << filepath << " reason : " << stbi_failure_reason();
// 			throw std::runtime_error("failed to load a texture");
// 		}

// 		createImageFromPtr(
// 			pixels,
// 			static_cast<uint32_t>(texWidth),
// 			static_cast<uint32_t>(texHeight),
// 			4
// 		);
		
// 		stbi_image_free(pixels);
// 	}

// 	void Texture::createImageFromPtr(void* data, uint32_t width, uint32_t height, int channels){
		
// 		_width = width;
// 		_height = height;
// 		VkDeviceSize imageSize = width * height * channels;

// 		Buffer stagingBuffer(_context);
// 		stagingBuffer.allocate(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

// 		stagingBuffer.map();
// 		stagingBuffer.writeToBuffer((void *)data);
// 		stagingBuffer.flush();
// 		stagingBuffer.unmap();


// 		VkImageCreateInfo imageInfo{};
// 		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
// 		imageInfo.imageType = VK_IMAGE_TYPE_2D;
// 		imageInfo.extent.width = width;
// 		imageInfo.extent.height = height;
// 		imageInfo.extent.depth = 1;
// 		imageInfo.mipLevels = 1;
// 		imageInfo.arrayLayers = 1;
// 		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
// 		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
// 		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
// 		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
// 		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
// 		imageInfo.flags = 0;

// 		if (vkCreateImage(_context.device().get(), &imageInfo, nullptr, &_image) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics.Texture") << "Failed to create texture image";
// 			throw std::runtime_error("failed to create image");
// 		}

// 		VkMemoryRequirements memRequirements;
// 		vkGetImageMemoryRequirements(_context.device().get(), _image, &memRequirements);

// 		VkMemoryAllocateInfo allocInfo{};
// 		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
// 		allocInfo.allocationSize = memRequirements.size;
// 		allocInfo.memoryTypeIndex = _context.device().findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

// 		if (vkAllocateMemory(_context.device().get(), &allocInfo, nullptr, &_memory) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics.Texture") << "Failed to allocate texture memory";
// 			throw std::runtime_error("failed to allocate image memory");
// 		}

// 		vkBindImageMemory(_context.device().get(), _image, _memory, 0);

// 		VkCommandBuffer commandBuffer = _context.transfert.commandPool.beginSingleTime();

// 		transitionToTransfert(commandBuffer);
// 		copyBuffer(commandBuffer, stagingBuffer);
// 		transitionToShaderRead(commandBuffer);

// 		_context.transfert.commandPool.endSingleTime(commandBuffer);
// 	}

// 	void Texture::transitionToTransfert(VkCommandBuffer commandBuffer){
// 		VkImageMemoryBarrier barrier{};
// 		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
// 		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
// 		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		barrier.image = _image;
// 		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		barrier.subresourceRange.baseMipLevel = 0;
// 		barrier.subresourceRange.levelCount = 1;
// 		barrier.subresourceRange.baseArrayLayer = 0;
// 		barrier.subresourceRange.layerCount = 1;
// 		barrier.srcAccessMask = 0;
// 		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

// 		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
// 	}

// 	void Texture::copyBuffer(VkCommandBuffer commandBuffer, Buffer& buffer){
// 		VkBufferImageCopy region{};
// 		region.bufferOffset = 0;
// 		region.bufferRowLength = 0;
// 		region.bufferImageHeight = 0;

// 		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		region.imageSubresource.mipLevel = 0;
// 		region.imageSubresource.baseArrayLayer = 0;
// 		region.imageSubresource.layerCount = 1;

// 		region.imageOffset = {0, 0, 0};
// 		region.imageExtent = {_width, _height, 1};

// 		vkCmdCopyBufferToImage(commandBuffer, buffer.get(), _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
// 	}

// 	void Texture::transitionToShaderRead(VkCommandBuffer commandBuffer){
// 		VkImageMemoryBarrier barrier{};
// 		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
// 		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
// 		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// 		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
// 		barrier.image = _image;
// 		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		barrier.subresourceRange.baseMipLevel = 0;
// 		barrier.subresourceRange.levelCount = 1;
// 		barrier.subresourceRange.baseArrayLayer = 0;
// 		barrier.subresourceRange.layerCount = 1;
// 		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
// 		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

// 		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
// 	}

// 	void Texture::createImageView(){
// 		VkImageViewCreateInfo info{};
// 		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
// 		info.format = VK_FORMAT_R8G8B8A8_SRGB;
// 		info.image = _image;
// 		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
// 		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
// 		info.subresourceRange.baseMipLevel = 0;
// 		info.subresourceRange.levelCount = 1;
// 		info.subresourceRange.baseArrayLayer = 0;
// 		info.subresourceRange.layerCount = 1;

// 		if (vkCreateImageView(_context.device().get(), &info, _context.allocationCallbacks, &_imageView) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics.Texture") << "Failed to create texture image view";
// 			throw std::runtime_error("failed to create texture image view");
// 		}
// 	}

// 	void Texture::createSampler(){
// 		VkSamplerCreateInfo info{};
// 		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
// 		info.magFilter = VK_FILTER_LINEAR;
// 		info.minFilter = VK_FILTER_LINEAR;
// 		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
// 		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
// 		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
// 		info.anisotropyEnable = VK_TRUE;
// 		info.maxAnisotropy = _context.device().properties().limits.maxSamplerAnisotropy;
// 		info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
// 		info.unnormalizedCoordinates = VK_FALSE;
// 		info.compareEnable = VK_FALSE;
// 		info.compareOp = VK_COMPARE_OP_ALWAYS;
// 		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
// 		info.mipLodBias = 0.f;
// 		info.minLod = 0.f;
// 		info.maxLod = 0.f;

// 		if (vkCreateSampler(_context.device().get(), &info, _context.allocationCallbacks, &_sampler) != VK_SUCCESS){
// 			CLOG(ERROR, "Engine.Graphics.Texture") << "Failed to create texture sampler";
// 			throw std::runtime_error("failed to create texture sampler");
// 		}
// 	}

// 	VkDescriptorImageInfo Texture::info() const{
// 		VkDescriptorImageInfo info;
// 		info.sampler = _sampler;
// 		info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
// 		info.imageView = _imageView;
// 		return info;
// 	} 

// }