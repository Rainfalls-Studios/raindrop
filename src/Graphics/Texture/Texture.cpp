#include <Raindrop/Graphics/Textures/Texture.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/ResourceExceptions.hpp>

#include <spdlog/spdlog.h>
#include <future>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Textures{
	void createStagingBuffer(Context& context, VkBuffer& buffer, VkDeviceMemory& memory, std::size_t size){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		{
			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = static_cast<VkDeviceSize>(size);
			info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			Exceptions::checkVkCreation<VkBuffer>(
				vkCreateBuffer(device.get(), &info, allocationCallbacks, &buffer),
				"Failed to create stagin buffer"
			);
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), buffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			Exceptions::checkVkCreation<VkDeviceMemory>(
				vkAllocateMemory(device.get(), &info, allocationCallbacks, &memory),
				"Failed to allocate stagin buffer memory"
			);

			Exceptions::checkVkOperation<VkBuffer>(
				vkBindBufferMemory(device.get(), buffer, memory, 0),
				"Failed to bind stagin buffer memory",
				Exceptions::VulkanOperationType::BINDING
			);
		}
	}

	void writeBuffer(Context& context, VkDeviceMemory& memory, const void* data, const std::size_t size){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		void* map;

		Exceptions::checkVkOperation<VkDeviceMemory>(
			vkMapMemory(device.get(), memory, 0, size, 0, &map),
			"Failed to map staging buffer memory",
			Exceptions::VulkanOperationType::MAPPING
		);

		memcpy(map, data, static_cast<size_t>(size));

		vkUnmapMemory(device.get(), memory);
	}

	void createImage(Context& context, VkImage& image, VkDeviceMemory& memory, std::size_t width, std::size_t height){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		{
			VkImageCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;

			info.extent.width = static_cast<uint32_t>(width);
			info.extent.height = static_cast<uint32_t>(height);
			info.extent.depth = 1;

			info.mipLevels = 1;
			info.arrayLayers = 1;

			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.format = VK_FORMAT_R8G8B8A8_SRGB;
			info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			Exceptions::checkVkCreation<VkImage>(
				vkCreateImage(device.get(), &info, nullptr, &image),
				"Failed to create texture image"
			);
		}

		{
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device.get(), image, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			Exceptions::checkVkCreation<VkDeviceMemory>(
				vkAllocateMemory(device.get(), &allocInfo, nullptr, &memory),
				"Failed to allocate texture image memory"
			);

			Exceptions::checkVkOperation<VkImage>(
				vkBindImageMemory(device.get(), image, memory, 0),
				"Failed to bind texture image memory",
				Exceptions::VulkanOperationType::BINDING
			);
		}
	}

	void createCommandBuffer(Context& context, VkCommandBuffer& commandBuffer){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		{
			VkCommandBufferAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			info.commandBufferCount = 1;
			info.commandPool = context.commandPools.singleUseTransfert.get();
			info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

			Exceptions::checkVkCreation<VkCommandBuffer>(
				vkAllocateCommandBuffers(device.get(), &info, &commandBuffer),
				"Failed to allocate texture transfert command buffer"
			);
		}

		{
			VkCommandBufferBeginInfo info{};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			Exceptions::checkVkOperation<VkCommandBuffer>(
				vkBeginCommandBuffer(commandBuffer, &info),
				"Failed to begin texture transfert command buffer",
				Exceptions::VulkanOperationType::BEGIN
			);
		}
	}

	void transfertToTransfert(const VkCommandBuffer& commandBuffer, const VkImage& image){
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;


		barrier.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};

		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

	void copyToImage(const VkCommandBuffer& commandBuffer, const VkImage& image, const VkBuffer& buffer, std::size_t width, std::size_t height){
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height),
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);
	}

	void transfertToReadOnly(const VkCommandBuffer& commandBuffer, const VkImage& image){
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;


		barrier.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};

		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

	void submitAndWait(Context& context, const VkCommandBuffer& commandBuffer){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		VkFence fence = VK_NULL_HANDLE;
		{
			VkFenceCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

			Exceptions::checkVkCreation<VkFence>(
				vkCreateFence(device.get(), &info, allocationCallbacks, &fence),
				"Failed to create texture submit fence"
			);
		}

		{
			vkEndCommandBuffer(commandBuffer);
			VkSubmitInfo info{};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 0;
			info.signalSemaphoreCount = 0;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &commandBuffer;
			
			Exceptions::checkVkOperation<VkCommandBuffer>(
				vkQueueSubmit(context.queues.transfertQueue(), 1, &info, fence),
				"Failed to submit texture transfert command buffer",
				Exceptions::VulkanOperationType::SUBMIT
			);
		}

		vkWaitForFences(device.get(), 1, &fence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(device.get(), fence, allocationCallbacks);
	}

	void Texture::loadFromConstructData(const ConstructData& data){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

		VkDeviceSize imageSize = data.width * data.height * data.channels;

		createStagingBuffer(_context, stagingBuffer, stagingBufferMemory, imageSize);
		writeBuffer(_context, stagingBufferMemory, data.data, imageSize);

		createImage(_context, _image, _memory, data.width, data.height);
		createImageView();
		createSampler();

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		
		createCommandBuffer(_context, commandBuffer);

		transfertToTransfert(commandBuffer, _image);
		copyToImage(commandBuffer, _image, stagingBuffer, data.width, data.height);
		transfertToReadOnly(commandBuffer, _image);

		submitAndWait(_context, commandBuffer);

		vkFreeMemory(device.get(), stagingBufferMemory, allocationCallbacks);
		vkDestroyBuffer(device.get(), stagingBuffer, allocationCallbacks);
		vkFreeCommandBuffers(device.get(), _context.commandPools.singleUseTransfert.get(), 1, &commandBuffer);
	}

	Texture::Texture(Context& context, const ConstructData& data) : 
			_context{context},
			_image{VK_NULL_HANDLE},
			_view{VK_NULL_HANDLE},
			_sampler{VK_NULL_HANDLE},
			_memory{VK_NULL_HANDLE}
		{
		loadFromConstructData(data);
	}

	Texture::Texture(Context& context, const std::filesystem::path& path) :
			_context{context},
			_image{VK_NULL_HANDLE},
			_view{VK_NULL_HANDLE},
			_memory{VK_NULL_HANDLE}
		{
		
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		spdlog::info("Loading image texture from \"{}\" ...", path.string());

		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(path.string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = texWidth * texHeight * 4;

		if (!pixels){
			spdlog::error("Failed to load texture image from \"{}\", reason : {}", path.string(), stbi_failure_reason());
			throw Exceptions::ResourceLoadException(path, "Texture", stbi_failure_reason());
		}

		ConstructData data;
		data.width = texWidth;
		data.height = texHeight;
		data.channels = 4;
		data.data = pixels;

		loadFromConstructData(data);

		stbi_image_free(pixels);
	}

	Texture::~Texture(){
		spdlog::info("Destroying image texture ...");
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_image != VK_NULL_HANDLE){
			vkDestroyImage(device.get(), _image, allocationCallbacks);
			_image = VK_NULL_HANDLE;
		}

		if (_view != VK_NULL_HANDLE){
			vkDestroyImageView(device.get(), _view, allocationCallbacks);
			_view = VK_NULL_HANDLE;
		}

		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}

		if (_sampler != VK_NULL_HANDLE){
			vkDestroySampler(device.get(), _sampler, allocationCallbacks);
			_sampler = VK_NULL_HANDLE;
		}
	}

	void Texture::createImageView(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		VkImageViewCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _image;
		info.components = {
			.r = VK_COMPONENT_SWIZZLE_R,
			.g = VK_COMPONENT_SWIZZLE_G,
			.b = VK_COMPONENT_SWIZZLE_B,
			.a = VK_COMPONENT_SWIZZLE_A
		};

		info.format = VK_FORMAT_R8G8B8A8_SRGB;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;

		info.subresourceRange = VkImageSubresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		};

		Exceptions::checkVkCreation<VkImageView>(
			vkCreateImageView(device.get(), &info, allocationCallbacks, &_view),
			"Failed to create texture image view"
		);
	}


	VkImage Texture::image() const{
		return _image;
	}

	VkImageView Texture::imageView() const{
		return _view;
	}

	VkDeviceMemory Texture::memory() const{
		return _memory;
	}

	void Texture::createSampler(){
		VkSamplerCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.unnormalizedCoordinates = VK_FALSE;

		info.magFilter = VK_FILTER_NEAREST;
		info.minFilter = VK_FILTER_NEAREST;

		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

		info.anisotropyEnable = VK_TRUE;
		info.maxAnisotropy = _context.physicalDevice.limits().maxSamplerAnisotropy;

		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		info.mipLodBias = 0.0f;
		info.minLod = 0.0f;
		info.maxLod = 0.0f;

		info.compareEnable = VK_FALSE;
		info.compareOp = VK_COMPARE_OP_ALWAYS;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		Exceptions::checkVkCreation<VkSampler>(
			vkCreateSampler(device.get(), &info, allocationCallbacks, &_sampler),
			"Failed to create texture image sampler"
		);
	}

	VkSampler Texture::sampler() const{
		return _sampler;
	}

	VkDescriptorImageInfo Texture::info() const{
		return VkDescriptorImageInfo{
			.sampler = _sampler,
			.imageView = _view,
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};
	}

}