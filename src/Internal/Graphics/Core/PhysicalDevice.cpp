#include <Raindrop_internal/Graphics/Core/PhysicalDevice.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <cstring>

#include <spdlog/spdlog.h>

#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics::Core{
	PhysicalDevice::PhysicalDevice(Context& context, VkPhysicalDevice device) : _context{context}, _device{device}{
		reset();
	}

	PhysicalDevice& PhysicalDevice::operator=(VkPhysicalDevice device){
		_device = device;
		reset();
		return *this;
	}
	
	uint32_t PhysicalDevice::apiVersion() const{
		return _properties.apiVersion;
	}

	uint32_t PhysicalDevice::driverVersion() const{
		return _properties.driverVersion;
	}

	uint32_t PhysicalDevice::vendorID() const{
		return _properties.vendorID;
	}

	uint32_t PhysicalDevice::deviceID() const{
		return _properties.deviceID;
	}

	VkPhysicalDeviceType PhysicalDevice::type() const{
		return _properties.deviceType;
	}

	const char* PhysicalDevice::name() const{
		return _properties.deviceName;
	}
	
	const VkPhysicalDeviceSparseProperties& PhysicalDevice::sparseProperties() const{
		return _properties.sparseProperties;
	}

	const VkPhysicalDeviceLimits& PhysicalDevice::limits() const{
		return _properties.limits;
	}

	const VkPhysicalDeviceFeatures& PhysicalDevice::features() const{
		return _features;
	}

	const VkPhysicalDeviceProperties& PhysicalDevice::properties() const{
		return _properties;
	}

	const VkPhysicalDeviceMemoryProperties& PhysicalDevice::memoryProperties() const{
		return _memoryProperties;
	}

	const std::vector<VkPhysicalDeviceToolProperties>& PhysicalDevice::toolProperties() const{
		return _toolProperties;
	}

	const std::vector<VkQueueFamilyProperties>& PhysicalDevice::queueFamilyProperties() const{
		return _queueFamilyProperties;
	}

	const std::vector<VkExtensionProperties>& PhysicalDevice::getSupportedExtensionProperties() const{
		return _supportedExtensionProperties;
	}

	const std::vector<VkLayerProperties>& PhysicalDevice::getSupportedLayerProperties() const{
		return _supportedLayerProperties;
	}

	bool PhysicalDevice::areFeaturesSupported(const VkPhysicalDeviceFeatures& features) const{
		const VkBool32* requiredFeaturesArray = reinterpret_cast<const VkBool32*>(&features);
		const VkBool32* enabledFeaturesArray = reinterpret_cast<const VkBool32*>(&_features);
		static constexpr uint32_t arraySize = sizeof(features) / sizeof(VkBool32);

		for (uint32_t i=0; i<arraySize; i++){
			if (requiredFeaturesArray[i] == VK_TRUE){
				if (enabledFeaturesArray[i] == VK_FALSE) return false;
			}
		}

		return true;
	}

	bool PhysicalDevice::isExtensionSupported(const char* extension) const{
		uint32_t count=0;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, nullptr),
			"Failed to enumerate physical device extension properties",
			Exceptions::VulkanOperationType::QUERYING
		);

		if (count == 0) return false;
		std::vector<VkExtensionProperties> extensions(count);

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, extensions.data()),
			"Failed to enumerate physical device extension properties",
			Exceptions::VulkanOperationType::QUERYING
		);
		
		for (auto &e : extensions){
			if (std::strcmp(e.extensionName, extension) == 0) return true;
		}
		return false;
	}

	bool PhysicalDevice::isLayerSupported(const char* layer) const{
		uint32_t count=0;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceLayerProperties(_device, &count, nullptr),
			"Failed to enumerate physical device layer properties",
			Exceptions::VulkanOperationType::QUERYING
		);

		if (count == 0) return false;
		std::vector<VkLayerProperties> layers(count);

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceLayerProperties(_device, &count, layers.data()),
			"Failed to enumerate physical device layer properties",
			Exceptions::VulkanOperationType::QUERYING
		);
		
		for (auto &l : layers){
			if (std::strcmp(l.layerName, layer) == 0) return true;
		}
		return false;
	}

	SwapchainSupport PhysicalDevice::getSwapchainSupport(VkSurfaceKHR surface) const{
		SwapchainSupport support;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, surface, &support.capabilities),
			"Failed to get physical device surface capabilities",
			Exceptions::VulkanOperationType::QUERYING
		);

		uint32_t count;
		
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, nullptr),
			"Failed to get physical device surface formats",
			Exceptions::VulkanOperationType::QUERYING
		);
		
		if (count){
			support.formats.resize(count);

			Exceptions::checkVkOperation<VkPhysicalDevice>(
				vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, support.formats.data()),
				"Failed to get physical device surface formats",
				Exceptions::VulkanOperationType::QUERYING
			);
		}

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, nullptr),
			"Failed to get physical device surface formats",
			Exceptions::VulkanOperationType::QUERYING
		);
		if (count){
			support.presentModes.resize(count);

			Exceptions::checkVkOperation<VkPhysicalDevice>(
				vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, support.presentModes.data()),
				"Failed to get physical device surface formats",
				Exceptions::VulkanOperationType::QUERYING
			);
		}

		support.supported = !support.formats.empty() && !support.presentModes.empty();
		return support;
	}

	VkPhysicalDevice PhysicalDevice::get() const{
		return _device;
	}

	void PhysicalDevice::reset(){
		if (_device == VK_NULL_HANDLE){
			_features = {};
			_properties = {};
			_memoryProperties = {};
			_toolProperties.clear();
			_queueFamilyProperties.clear();
			_supportedExtensionProperties.clear();
			_supportedLayerProperties.clear();
		} else {
			getFeatures();
			getProperties();
			getMemoryProperties();
			getToolProperties();
			getFamilyProperties();
			getExtensionProperties();
			getLayerProperties();
		}
	}
	
	void PhysicalDevice::getFeatures(){
		vkGetPhysicalDeviceFeatures(_device, &_features);
	}

	void PhysicalDevice::getProperties(){
		vkGetPhysicalDeviceProperties(_device, &_properties);
	}

	void PhysicalDevice::getMemoryProperties(){
		vkGetPhysicalDeviceMemoryProperties(_device, &_memoryProperties);
	}

	void PhysicalDevice::getToolProperties(){
		if (_context.getInstance().API_VERSION != VK_API_VERSION_1_3){
			_context.getLogger()->warn("vkGetPhysicalDeviceToolProperties is only available in Vulkan 1.3.0");
			return;
		}
		
		uint32_t count;
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceToolProperties(_device, &count, nullptr),
			"Failed to get physical device tool properties",
			Exceptions::VulkanOperationType::QUERYING
		);

		_toolProperties.resize(count);
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceToolProperties(_device, &count, _toolProperties.data()),
			"Failed to get physical device tool properties",
			Exceptions::VulkanOperationType::QUERYING
		);
	}

	void PhysicalDevice::getFamilyProperties(){
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);

		_queueFamilyProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, _queueFamilyProperties.data());
	}

	void PhysicalDevice::getExtensionProperties(){
		uint32_t count;

		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, nullptr),
			"Failed to get physical extensions properties",
			Exceptions::VulkanOperationType::QUERYING
		);

		_supportedExtensionProperties.resize(count);
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, _supportedExtensionProperties.data()),
			"Failed to get physical extensions properties",
			Exceptions::VulkanOperationType::QUERYING
		);
	}

	void PhysicalDevice::getLayerProperties(){
		uint32_t count;
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceLayerProperties(_device, &count, nullptr),
			"Failed to get physical extensions properties",
			Exceptions::VulkanOperationType::QUERYING
		);

		_supportedLayerProperties.resize(count);
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceLayerProperties(_device, &count, _supportedLayerProperties.data()),
			"Failed to get physical extensions properties",
			Exceptions::VulkanOperationType::QUERYING
		);
	}
}