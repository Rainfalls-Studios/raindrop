#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	ImageConfigInfo::ImageConfigInfo() : 
		format{VK_FORMAT_UNDEFINED},
		extent{
			.width = 1,
			.height = 1,
			.depth = 1,
		},
		usage{0},
		layout{VK_IMAGE_LAYOUT_UNDEFINED},
		mipLevels{1},
		arrLayers{1},
		sampleCount{VK_SAMPLE_COUNT_1_BIT},
		tiling{VK_IMAGE_TILING_OPTIMAL},
		type{VK_IMAGE_TYPE_2D},
		features{0},
		flags{0},
		sharingMode{VK_SHARING_MODE_EXCLUSIVE},
		queueFamilies{}
	{}


	Image::Image(Context& context, const ImageConfigInfo& config) :
			_context{context},
			_image{VK_NULL_HANDLE},
			_format{VK_FORMAT_UNDEFINED},
			_memory{VK_NULL_HANDLE},
			_extent{}
		{

		_context.getLogger().info("Creating new image...");
		
		_format = config.format;
		_extent = config.extent;

		VkImageCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		info.extent = _extent;
		info.format = _format;
		info.arrayLayers = config.arrLayers;
		info.mipLevels = config.mipLevels;
		info.flags = config.flags;
		info.imageType = config.type;
		info.initialLayout = config.layout;
		info.pQueueFamilyIndices = config.queueFamilies.data();
		info.queueFamilyIndexCount = static_cast<uint32_t>(config.queueFamilies.size());
		info.samples = config.sampleCount;
		info.tiling = config.tiling;
		info.usage = config.usage;
		info.sharingMode = config.sharingMode;

		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

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


		_context.getLogger().info("Image created successfully !");
	}

	Image::~Image(){
		_context.getLogger().info("Destroying image");
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_image != VK_NULL_HANDLE){
			vkDestroyImage(device.get(), _image, allocationCallbacks);
			_image = VK_NULL_HANDLE;
		}

		if (_memory != VK_NULL_HANDLE){
			vkFreeMemory(device.get(), _memory, allocationCallbacks);
			_memory = VK_NULL_HANDLE;
		}


		_context.getLogger().info("Image destroyed successfully !");
	}
	
	VkImage Image::get() const{
		return _image;
	}

	VkDeviceMemory Image::memory() const{
		return _memory;
	}

	VkFormat Image::format() const{
		return _format;
	}

	VkExtent3D Image::extent() const{
		return _extent;
	}
}