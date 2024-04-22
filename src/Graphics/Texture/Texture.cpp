#include <Raindrop/Graphics/Textures/Texture.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/ResourceExceptions.hpp>

#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Sampler.hpp>
#include <Raindrop/Graphics/Buffer.hpp>

#include <spdlog/spdlog.h>
#include <future>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics::Textures{
	
	void writeBuffer(Context& context, Buffer& buffer, const void* data, const std::size_t size){
		auto& device = context.device;
		auto& allocationCallbacks = context.allocationCallbacks;

		void* map;

		Exceptions::checkVkOperation<VkBuffer>(
		 	buffer.map(),
			"Failed to map staging buffer",
			Exceptions::VulkanOperationType::MAPPING
		);

		buffer.write(data, size);
		
		Exceptions::checkVkOperation<VkBuffer>(
			buffer.flush(),
			"Failed to flush staging buffer",
			Exceptions::VulkanOperationType::FLUSHING
		);

		buffer.unmap();

	}

	void Texture::createImage(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;
		
		ImageConfigInfo info{};

		info.extent = {
			.width = _size.x,
			.height = _size.y,
			.depth = 1
		};

		info.layout = VK_IMAGE_LAYOUT_UNDEFINED;
		info.format = VK_FORMAT_R8G8B8A8_SRGB;
		info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		_image = std::make_shared<Image>(_context, info);
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

	void transfertToTransfert(const VkCommandBuffer& commandBuffer, const std::shared_ptr<Image>& image){
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image->get();


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

	void copyToImage(const VkCommandBuffer& commandBuffer, const std::shared_ptr<Image>& image, const Buffer& buffer, std::size_t width, std::size_t height){
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
			buffer.get(),
			image->get(),
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);
	}

	void transfertToReadOnly(const VkCommandBuffer& commandBuffer, const std::shared_ptr<Image>& image){
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image->get();


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

		VkDeviceSize imageSize = data.width * data.height * data.channels;

		_size.x = data.width;
		_size.y = data.height;

		Buffer stagingBuffer(_context);

		stagingBuffer.allocate(
			imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);

		writeBuffer(_context, stagingBuffer, data.data, imageSize);

		createImage();
		createImageView();
		createSampler();

		VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
		
		createCommandBuffer(_context, commandBuffer);

		transfertToTransfert(commandBuffer, _image);
		copyToImage(commandBuffer, _image, stagingBuffer, data.width, data.height);
		transfertToReadOnly(commandBuffer, _image);

		submitAndWait(_context, commandBuffer);

		vkFreeCommandBuffers(device.get(), _context.commandPools.singleUseTransfert.get(), 1, &commandBuffer);
	}

	Texture::Texture(Context& context, const ConstructData& data) : 
			_context{context},
			_image{nullptr},
			_imageView{nullptr},
			_sampler{nullptr}
		{
		loadFromConstructData(data);
	}

	Texture::Texture(Context& context, const std::filesystem::path& path) :
			_context{context},
			_image{nullptr},
			_imageView{nullptr},
			_sampler{nullptr}
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

		_imageView.reset();
		_image.reset();
		_sampler.reset();
	}

	void Texture::createImageView(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		ImageViewConfigInfo info{};

		info.image = _image;

		info.format = VK_FORMAT_R8G8B8A8_SRGB;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		_imageView = std::make_shared<ImageView>(_context, info);
	}

	const std::shared_ptr<Image>& Texture::image() const{
		return _image;
	}

	const std::shared_ptr<ImageView>& Texture::imageView() const{
		return _imageView;
	}

	void Texture::createSampler(){
		SamplerConfigInfo info{};

		info.unnormalizedCoordinates = VK_FALSE;

		info.anisotropyEnable = VK_TRUE;
		info.maxAnisotropy = _context.physicalDevice.limits().maxSamplerAnisotropy;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		_sampler = std::make_shared<Sampler>(_context, info);
	}

	const std::shared_ptr<Sampler>& Texture::sampler() const{
		return _sampler;
	}

	VkDescriptorImageInfo Texture::info() const{
		assert(_sampler && _imageView);
		return VkDescriptorImageInfo{
			.sampler = _sampler->get(),
			.imageView = _imageView->get(),
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		};
	}

}