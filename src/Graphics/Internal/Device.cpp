#include <Raindrop/Graphics/Internal/Device.hpp>
#include <Raindrop/Graphics/Internal/Context.hpp>

namespace Raindrop::Graphics::Internal{
	Device::Device(Context& context) : _context{context}{
		_context.logger.info("Initializing vulkan device...");

		findPhysicalDevice();
		vkGetPhysicalDeviceProperties(_physicalDevice.get(), &_properties);
		build();

		_context.logger.info("Vulkan device initialized without any critical error");
	}

	Device::~Device(){
		_context.logger.info("Terminating vulkan device...");
		auto& allocationCallbacks = _context.graphics.allocationCallbacks;
		vkDestroyDevice(_device, allocationCallbacks);
		_context.logger.info("Vulkan device terminated without any critical error");
	}

	void Device::findPhysicalDevice(){
		_context.logger.trace("Looking for suitable physical device...");
		uint32_t count = 0;
		vkEnumeratePhysicalDevices(_context.instance.get(), &count, nullptr);

		if (count == 0){
			_context.logger.error("Cannot find any graphics card with vulkan support");
			throw std::runtime_error("Failed to find physical device with Vulkan support");
		}

		std::vector<VkPhysicalDevice> physicalDevices(count);
		vkEnumeratePhysicalDevices(_context.instance.get(), &count, physicalDevices.data());

		for (auto &device : physicalDevices){
			if (isPhysicalDeviceSuitable(device)){
				_physicalDevice = device;
				return;
			}
		}

		_context.logger.error("Cannot find a suitable physical device");
		throw std::runtime_error("Failed to find a suitable physical device");
	}

	bool Device::isPhysicalDeviceSuitable(VkPhysicalDevice device){
		uint32_t surfaceFormatCount = 0;
		uint32_t presentModeCount = 0;

		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _context.window.surface(), &surfaceFormatCount, nullptr);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _context.window.surface(), &presentModeCount, nullptr);

		return surfaceFormatCount && presentModeCount;
	}
	
	VkDevice Device::get() const{
		return _device;
	}

	const PhysicalDevice& Device::getPhysicalDevice() const{
		return _physicalDevice;
	}

	void Device::build(){
		auto& allocationCallbacks = _context.graphics.allocationCallbacks;

		if (!isExtensionsSupported()){
			_context.logger.error("Failed to build a vulkan device, unsuported required extensions");
			throw std::runtime_error("unsupported extensions");
		}

		if (!isLayersSupported()){
			_context.logger.error("Failed to build a vulkan device, unsuported required layers");
			throw std::runtime_error("unsupported layers");
		}

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		const VkPhysicalDeviceFeatures& features = _physicalDevice.features();

		info.ppEnabledExtensionNames = REQUIRED_EXTENSIONS.data();
		info.ppEnabledLayerNames = REQUIRED_LAYERS.data();

		info.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_EXTENSIONS.size());
		info.enabledLayerCount = static_cast<uint32_t>(REQUIRED_LAYERS.size());

		const std::vector<VkQueueFamilyProperties>& familyProperties = _physicalDevice.queueFamilyProperties();
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(familyProperties.size());
		std::vector<std::vector<float>> priorities(familyProperties.size());

		for (uint32_t i=0; i<static_cast<uint32_t>(queueCreateInfos.size()); i++){
			auto& info = queueCreateInfos[i];
			auto& priority = priorities[i];
			const auto& properties = familyProperties[i];

			priority.resize(properties.queueCount);
			std::fill(priority.begin(), priority.end(), 0.f);

			info = {};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.queueFamilyIndex = i;
			info.queueCount = properties.queueCount;
			info.pQueuePriorities = priority.data();
		}

		info.pQueueCreateInfos = queueCreateInfos.data();
		info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		if (vkCreateDevice(_physicalDevice.get(), &info, allocationCallbacks, &_device) != VK_SUCCESS){
			_context.logger.error("Failed to create vulkan device");
			throw std::runtime_error("failed to create vulkan device");
		}

		// _context.graphics.familyIndex = getGraphicsFamily();
		// _context.present.familyIndex = getPresentFamily();
		// _context.transfert.familyIndex = getTransfertFamily();

		// vkGetDeviceQueue(_device, _context.graphics.familyIndex, 0, &_context.graphics.queue);
		// vkGetDeviceQueue(_device, _context.present.familyIndex, 0, &_context.present.queue);

		// if (_context.graphics.familyIndex == _context.transfert.familyIndex || _context.present.familyIndex == _context.transfert.familyIndex){
		// 	if (familyProperties[_context.transfert.familyIndex].queueCount > 1){
		// 		vkGetDeviceQueue(_device, _context.transfert.familyIndex, 1, &_context.transfert.queue);
		// 	} else {
		// 		vkGetDeviceQueue(_device, _context.transfert.familyIndex, 0, &_context.transfert.queue);
		// 	}
		// } else {
		// 	vkGetDeviceQueue(_device, _context.transfert.familyIndex, 0, &_context.transfert.queue);
		// }
	}

	bool Device::isExtensionsSupported(){
		for (const auto &extension : REQUIRED_EXTENSIONS){
			if (!isExtensionSupported(extension)) return false;
		}
		return true;
	}

	bool Device::isLayersSupported(){
		for (const auto &layer : REQUIRED_LAYERS){
			if (!isLayerSupported(layer)) return false;
		}
		return true;
	}

	bool Device::isExtensionSupported(const char* extensionName){
		return _physicalDevice.isExtensionSupported(extensionName);
	}

	bool Device::isLayerSupported(const char* layerName){
		return _physicalDevice.isLayerSupported(layerName);
	}

	uint32_t Device::getGraphicsFamily(){
		const auto& properties = _physicalDevice.queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::getTransfertFamily(){
		const auto& properties = _physicalDevice.queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
				return i;
			}
		}
		throw std::runtime_error("failed to find a graphics family");
	}

	uint32_t Device::getPresentFamily(){
		const auto& properties = _physicalDevice.queueFamilyProperties();
		
		for (uint32_t i=0; i<properties.size(); i++){
			VkBool32 supported;
			vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice.get(), i, _context.window.surface(), &supported);
			return i;
		}
		throw std::runtime_error("failed to find a present family");
	}

	uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(_physicalDevice.get(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		_context.logger.warn("Failed to find suitable memory type filter (filter : %x; properties: %x)", typeFilter, properties);
		throw std::runtime_error("failed to find suitable memory type");
	}

	SwapchainSupport Device::getSwapchainSupport(VkSurfaceKHR surface) const{
		SwapchainSupport support;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_physicalDevice.get(), surface, &support.capabilities);

		uint32_t count;

		vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice.get(), surface, &count, nullptr);
		if (count){
			support.formats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_physicalDevice.get(), surface, &count, support.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice.get(), surface, &count, nullptr);
		if (count){
			support.presentModes.resize(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_physicalDevice.get(), surface, &count, support.presentModes.data());
		}

		support.supported = !support.formats.empty() && !support.presentModes.empty();
		return support;
	}

	void Device::waitIdle(){
		vkDeviceWaitIdle(_device);
	}

	const VkPhysicalDeviceProperties& Device::properties() const{
		return _properties;
	}
}