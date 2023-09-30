#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Builders/ImageBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics{
	Image::Image(GraphicsContext& context, const Builders::ImageBuilder& builder) : _context{context}{
		_image = VK_NULL_HANDLE;
		_memory = VK_NULL_HANDLE;

		VkImageCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.arrayLayers = builder.arrayLayers();
		info.extent = builder.extent();
		info.flags = 0;
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

		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (vkCreateImage(device, &info, allocationCallbacks, &_image) != VK_SUCCESS){
			throw std::runtime_error("Failed to create image");
		}

		VkMemoryRequirements requirements;
		vkGetImageMemoryRequirements(device, _image, &requirements);

		VkMemoryAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocationInfo.memoryTypeIndex = _context.device.findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		allocationInfo.allocationSize = requirements.size;

		if (vkAllocateMemory(device, &allocationInfo, allocationCallbacks, &_memory) != VK_SUCCESS){
			throw std::runtime_error("Failed to allocate image memory");
		}

		if (vkBindImageMemory(_context.device.get(), _image, _memory, 0) != VK_SUCCESS){
			throw std::runtime_error("Failed to bind image memory");
		}
	}

	Image::~Image(){
		auto device = _context.device.get();
		auto allocationCallbacks = _context.allocationCallbacks;

		if (_image) vkDestroyImage(device, _image, allocationCallbacks);
		if (_memory) vkFreeMemory(device, _memory, allocationCallbacks);

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

	std::shared_ptr<Image> Image::create(GraphicsContext& context, const Builders::ImageBuilder& builder){
		return std::make_shared<Image>(context, builder);
	}
}