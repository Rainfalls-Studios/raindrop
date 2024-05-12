#include <Raindrop_internal/Graphics/Core/Device.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics::Core{
	Device::Device(Context& context) : _context{context}{
		_context.getLogger()->info("Constructing vulkan device ...");

		findPhysicalDevice();
		build();
	}

	Device::~Device(){
		_context.getLogger()->info("Destroying vulkan device ...");

		vkDestroyDevice(_device, _context.getAllocationCallbacks());
	}

	void Device::findPhysicalDevice(){
		_context.getLogger()->trace("Looking fo suitable physical device ...");

		const auto& instance = _context.getInstance().get();

		uint32_t count = 0;
		Exceptions::checkVkOperation<VkInstance>(
			vkEnumeratePhysicalDevices(instance, &count, nullptr),
			"Failed to enumerate physical devices",
			Exceptions::VulkanOperationType::QUERYING
		);

		if (count == 0){
			_context.getLogger()->error("Failed to find a suitable physical device with vulkan support");
			throw std::runtime_error("Failed to find physical device with Vulkan support");
		}

		std::vector<VkPhysicalDevice> physicalDevices(count);
		Exceptions::checkVkOperation<VkInstance>(
			vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data()),
			"Failed to enumerate physical devices",
			Exceptions::VulkanOperationType::QUERYING
		);

		for (auto &device : physicalDevices){
			if (isPhysicalDeviceSuitable(device)){
				_context.getPhysicalDevice() = device;

				_context.getLogger()->info("Found physical device : {}", _context.getPhysicalDevice().name());
				return;
			}
		}

		_context.getLogger()->error("Failed to find a suitable physical device");
		throw std::runtime_error("Failed to find a suitable physical device");
	}

	bool Device::isPhysicalDeviceSuitable(VkPhysicalDevice device){
		uint32_t surfaceFormatCount = 0;
		uint32_t presentModeCount = 0;

		Exceptions::checkVkOperation<VkInstance>(
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, _context.getWindow().surface(), &surfaceFormatCount, nullptr),
			"Failed to get physical device surface formats",
			Exceptions::VulkanOperationType::QUERYING
		);

		Exceptions::checkVkOperation<VkInstance>(
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, _context.getWindow().surface(), &presentModeCount, nullptr),
			"Failed to get physical device surface present modes",
			Exceptions::VulkanOperationType::QUERYING
		);

		return surfaceFormatCount && presentModeCount;
	}
	
	VkDevice Device::get() const{
		return _device;
	}

	void Device::build(){
		auto& allocationCallbacks = _context.getAllocationCallbacks();
		auto& physicalDevice = _context.getPhysicalDevice();

		if (!isExtensionsSupported()){
			_context.getLogger()->error("Failed to build a vulkan device, unsupported required extensions");
			throw std::runtime_error("unsupported extensions");
		}

		if (!isLayersSupported()){
			_context.getLogger()->error("Failed to build a vulkan device, unsupported required layers");
			throw std::runtime_error("unsupported layers");
		}

		VkDeviceCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		
		const VkPhysicalDeviceFeatures& features = physicalDevice.features();

		info.ppEnabledExtensionNames = REQUIRED_EXTENSIONS;
		info.ppEnabledLayerNames = REQUIRED_LAYERS;

		info.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_EXTENSIONS_COUNT);
		info.enabledLayerCount = static_cast<uint32_t>(REQUIRED_LAYERS_COUNT);

		const std::vector<VkQueueFamilyProperties>& familyProperties = physicalDevice.queueFamilyProperties();
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

		info.pEnabledFeatures = &features;
		info.pQueueCreateInfos = queueCreateInfos.data();
		info.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		Exceptions::checkVkCreation<VkDevice>(
			vkCreateDevice(physicalDevice.get(), &info, allocationCallbacks, &_device),
			"Failed to create device"
		);
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
		return _context.getPhysicalDevice().isExtensionSupported(extensionName);
	}

	bool Device::isLayerSupported(const char* layerName){
		return _context.getPhysicalDevice().isLayerSupported(layerName);
	}

	uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){

		auto& physicalDevice = _context.getPhysicalDevice();
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice.get(), &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		_context.getLogger()->warn("Failed to find suitable memory type filter (filter : {}; properties: {})", typeFilter, properties);
		throw std::runtime_error("failed to find suitable memory type");
	}

	void Device::waitIdle(){
		vkDeviceWaitIdle(_device);
	}
}