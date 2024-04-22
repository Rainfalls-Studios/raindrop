#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	Image::Image(Context& context, const ImageConfigInfo& config) : _context{context}{
		_image = VK_NULL_HANDLE;
		_memory = VK_NULL_HANDLE;

		_format = config.format;
		_extent = config.extent;
		_mipLevels = config.mipLevels;
		_arrLayers = config.arrLayers;

		VkImageCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.arrayLayers =_arrLayers;
		info.extent = _extent;
		info.format = _format;
		info.mipLevels = _mipLevels;
		info.flags = config.flags;
		info.imageType = config.type;
		info.initialLayout = config.layout;
		info.pQueueFamilyIndices = config.queueFamilies.data();
		info.queueFamilyIndexCount = static_cast<uint32_t>(config.queueFamilies.size());
		info.samples = config.sampleCount;
		info.tiling = config.tiling;
		info.usage = config.usage;
		info.sharingMode = config.queueFamilies.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;

		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		Exceptions::checkVkCreation<VkImage>(
			vkCreateImage(device.get(), &info, allocationCallbacks, &_image),
			"Failed to create image"
		);

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device.get(), _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;

		Exceptions::checkVkCreation<VkDeviceMemory>(
			vkAllocateMemory(device.get(), &allocationInfo, allocationCallbacks, &_memory),
			"Failed to allocate image memory"
		);

		Exceptions::checkVkOperation<VkImage>(
			vkBindImageMemory(device.get(), _image, _memory, 0),
			"Failed to bind image memory",
			Exceptions::VulkanOperationType::BINDING
		);
	}

	Image::~Image(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_image != VK_NULL_HANDLE){
			vkDestroyImage(device.get(), _image, allocationCallbacks);
			_image = VK_NULL_HANDLE;
		}

		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}

	}
	
	VkImage Image::image() const{
		return _image;
	}

	VkFormat Image::format() const{
		return _format;
	}

	VkExtent3D Image::extent() const{
		return _extent;
	}
	
	uint32_t Image::mipLevels() const{
		return _mipLevels;
	}

	uint32_t Image::arrayLayers() const{
		return _arrLayers;
	}
}