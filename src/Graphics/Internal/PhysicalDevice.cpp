#include <Raindrop/Graphics/Internal/PhysicalDevice.hpp>
#include <cstring>

namespace Raindrop::Graphics::Internal{
	PhysicalDevice::PhysicalDevice(VkPhysicalDevice device) : _device{device}{
		reset();
	}

	PhysicalDevice& PhysicalDevice::operator=(VkPhysicalDevice device){
		_device = device;
		reset();
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
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, nullptr);

		if (count == 0) return false;
		std::vector<VkExtensionProperties> extensions(count);
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, extensions.data());
		
		for (auto &e : extensions){
			if (std::strcmp(e.extensionName, extension) == 0) return true;
		}
		return false;
	}

	bool PhysicalDevice::isLayerSupported(const char* layer) const{
		uint32_t count=0;
		vkEnumerateDeviceLayerProperties(_device, &count, nullptr);

		if (count == 0) return false;
		std::vector<VkLayerProperties> layers(count);
		vkEnumerateDeviceLayerProperties(_device, &count, layers.data());
		
		for (auto &l : layers){
			if (std::strcmp(l.layerName, layer) == 0) return true;
		}
		return false;
	}

	SwapchainSupport PhysicalDevice::getSwapchainSupport(VkSurfaceKHR surface) const{
		SwapchainSupport support;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, surface, &support.capabilities);

		uint32_t count;

		vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, nullptr);
		if (count){
			support.formats.resize(count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(_device, surface, &count, support.formats.data());
		}

		vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, nullptr);
		if (count){
			support.presentModes.resize(count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(_device, surface, &count, support.presentModes.data());
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
		uint32_t count;
		vkGetPhysicalDeviceToolProperties(_device, &count, nullptr);

		_toolProperties.resize(count);
		vkGetPhysicalDeviceToolProperties(_device, &count, _toolProperties.data());
	}

	void PhysicalDevice::getFamilyProperties(){
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);

		_queueFamilyProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, _queueFamilyProperties.data());
	}

	void PhysicalDevice::getExtensionProperties(){
		uint32_t count;
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, nullptr);

		_supportedExtensionProperties.resize(count);
		vkEnumerateDeviceExtensionProperties(_device, nullptr, &count, _supportedExtensionProperties.data());
	}

	void PhysicalDevice::getLayerProperties(){
		uint32_t count;
		vkEnumerateDeviceLayerProperties(_device, &count, nullptr);

		_supportedLayerProperties.resize(count);
		vkEnumerateDeviceLayerProperties(_device, &count, _supportedLayerProperties.data());
	}
}