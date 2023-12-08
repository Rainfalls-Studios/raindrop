#include <Raindrop/Graphics/Image/Image.hpp>
#include <Raindrop/Graphics/Image/ImageBuilder.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>

namespace Raindrop::Graphics::Image{
	Image::Image(Context& context, const ImageBuilder& builder) : _context{context}{
		_image = VK_NULL_HANDLE;
		_memory = VK_NULL_HANDLE;

		_format = builder.format();
		_extent = builder.extent();
		_mipLevels = builder.mipLevels();
		_arrLayers = builder.arrayLayers();

		_context.logger().info("Building \"{}\" image", builder.name());
		VkImageCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.arrayLayers = builder.arrayLayers();
		info.extent = builder.extent();
		info.flags = builder.flags();
		info.format = builder.format();
		info.imageType = builder.type();
		info.initialLayout = builder.layout();
		info.mipLevels = builder.mipLevels();
		info.pQueueFamilyIndices = builder.queueFamilyIndices();
		info.queueFamilyIndexCount = builder.queueFamilyCount();
		info.samples = builder.sampleCount();
		info.sharingMode = builder.sharingMode();
		info.tiling = builder.tiling();
		info.usage = builder.usage();

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (vkCreateImage(device.get(), &info, allocationCallbacks, &_image) != VK_SUCCESS){
			_context.logger().error("Failed to create vulkan image");
			throw std::runtime_error("Failed to create image");
		}

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device.get(), _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;

		if (vkAllocateMemory(device.get(), &allocationInfo, allocationCallbacks, &_memory) != VK_SUCCESS){
			_context.logger().error("Failed to allocate vulkan image memory");
			throw std::runtime_error("Failed to allocate image memory");
		}

		if (vkBindImageMemory(device.get(), _image, _memory, 0) != VK_SUCCESS){
			_context.logger().error("Failed to bind vulkan image memory");
			throw std::runtime_error("Failed to bind image memory");
		}
	}

	Image::~Image(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_image) vkDestroyImage(device.get(), _image, allocationCallbacks);
		if (_memory) vkFreeMemory(device.get(), _memory, allocationCallbacks);

		_image = VK_NULL_HANDLE;
		_memory = VK_NULL_HANDLE;
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