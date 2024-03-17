#include <Raindrop/Renderer/Texture/Texture.hpp>
#include <Raindrop/Renderer/Context.hpp>

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Raindrop::Renderer::Texture{
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
			spdlog::error("Failed to load texture image from \"{}\" !", path.string());
			throw std::runtime_error("failed to load texture image!");
		}

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;

		{
			VkBufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.size = imageSize;
			info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			if (vkCreateBuffer(device.get(), &info, allocationCallbacks, &stagingBuffer) != VK_SUCCESS){
				spdlog::error("Failed to create staging buffer !");
				throw std::runtime_error("Failed to create staging buffer");
			}
		}

		{
			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(device.get(), stagingBuffer, &requirements);

			VkMemoryAllocateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = requirements.size;
			info.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			
			if (vkAllocateMemory(device.get(), &info, allocationCallbacks, &stagingBufferMemory) != VK_SUCCESS){
				spdlog::error("Failed to allocate staging buffer memory !");
				throw std::runtime_error("Failed to allocate staging buffer memory");
			}

			if (vkBindBufferMemory(device.get(), stagingBuffer, stagingBufferMemory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind staging buffer memory !");
				throw std::runtime_error("Failed to bind staging buffer memory");
			}
		}

		{
			void* data;

			if (vkMapMemory(device.get(), stagingBufferMemory, 0, imageSize, 0, &data) != VK_SUCCESS){
				spdlog::error("Failed to map staging buffer memory !");
				throw std::runtime_error("Failed to map staging buffer memory");
			}

			memcpy(data, pixels, static_cast<size_t>(imageSize));
			vkUnmapMemory(device.get(), stagingBufferMemory);
		}

		stbi_image_free(pixels);

		{
			VkImageCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;

			info.extent.width = static_cast<uint32_t>(texWidth);
			info.extent.height = static_cast<uint32_t>(texHeight);
			info.extent.depth = 1;

			info.mipLevels = 1;
			info.arrayLayers = 1;

			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.format = VK_FORMAT_R8G8B8A8_SRGB;
			info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateImage(device.get(), &info, nullptr, &_image) != VK_SUCCESS) {
				spdlog::error("Failed to create image !");
				throw std::runtime_error("Failed to create image");
			}
		}

		{
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device.get(), _image, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			if (vkAllocateMemory(device.get(), &allocInfo, nullptr, &_memory) != VK_SUCCESS) {
				spdlog::error("Failed to allocate image memory !");
				throw std::runtime_error("Failed to allocate image memory");
			}

			if (vkBindImageMemory(device.get(), _image, _memory, 0) != VK_SUCCESS){
				spdlog::error("Failed to bind image memory !");
				throw std::runtime_error("Failed to bind image memory");
			}
		}

		{
			vkFreeMemory(device.get(), stagingBufferMemory, allocationCallbacks);
			vkDestroyBuffer(device.get(), stagingBuffer, allocationCallbacks);
		}
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
	}

	VkImage Texture::image(){
		return _image;
	}

	VkImageView Texture::imageView(){
		return _view;
	}

	VkDeviceMemory Texture::memory(){
		return _memory;
	}
}