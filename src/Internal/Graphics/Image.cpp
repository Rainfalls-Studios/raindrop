#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	Image::Image(Context& context, const VkImageCreateInfo& info) :
			_context{context},
			_image{VK_NULL_HANDLE},
			_memory{VK_NULL_HANDLE}
		{

		_context.getLogger()->info("Creating new image...");

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


		_context.getLogger()->info("Image created successfully !");
	}

	Image::~Image(){
		_context.getLogger()->info("Destroying image...");
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


		_context.getLogger()->info("Image destroyed successfully !");
	}
	
	VkImage Image::get() const{
		return _image;
	}

	VkDeviceMemory Image::memory() const{
		return _memory;
	}
}