#include <Raindrop/Graphics/Core/PhysicalDevice.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

#include <cstring>
#include <spdlog/spdlog.h>


namespace Raindrop::Graphics::Core{
	PhysicalDevice::PhysicalDevice() noexcept: 
		_context{nullptr},
		_device{VK_NULL_HANDLE},
		_surface{VK_NULL_HANDLE}
	{}

	PhysicalDevice::~PhysicalDevice(){
		release();
	}
	
	void PhysicalDevice::initialize(Context& context){
		_context = &context;

		findPhysicalDevice();
		queryProperties();
		queryFeatures();
		queryQueueProperties();

		_context->logger->info("Found physical device {} ", _properties.deviceName);
	}

	void PhysicalDevice::release(){
		_device = VK_NULL_HANDLE;
		_context = nullptr;
	}	

	VkPhysicalDevice PhysicalDevice::get() const noexcept{
		return _device;
	}

	void PhysicalDevice::findPhysicalDevice(){
		const auto& instance = _context->instance.get();

		// Get the number of available physical devices
		uint32_t count = 0;
		Exceptions::checkVkOperation<VkInstance>(
			vkEnumeratePhysicalDevices(instance, &count, nullptr),
			"Failed to enumerate physical devices",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		// If vulkan cannot find a single viable GPU, throw an exception
		if (count == 0){
			_context->logger->error("Failed to find a suitable physical device with vulkan support");
			throw std::runtime_error("Failed to find physical device with Vulkan support");
		}
		_context->logger->trace("Found {} physical device(s)", count);

		// Get the those physical devices
		std::vector<VkPhysicalDevice> physicalDevices(count);
		Exceptions::checkVkOperation<VkInstance>(
			vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data()),
			"Failed to enumerate physical devices",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		// Parse through the devices
		for (auto& device : physicalDevices){
			if (isDeviceSuitable(device)){
				_device = device;
				return;
			}
		}

		// If none of the devices are suitable, throw an exception
		_context->logger->error("Failed to find a suitable physical device");
		throw std::runtime_error("Failed to find a suitable physical device");
	}

	bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device){
		bool suitable = true;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		_context->logger->trace("Trying physical device {}", properties.deviceName);

		// If any check fails, the device is considered unsuitable
		suitable &= isSurfaceSupported(device);
		suitable &= areExtensionsSupported(device);

		_context->logger->trace("The physical device is {}", suitable ? "suitable" : "not suitable");;

		return suitable;
	}
	
	bool PhysicalDevice::isSurfaceSupported(VkPhysicalDevice device){
		if (_surface == VK_NULL_HANDLE){
			_context->logger->trace("Surface support not required : Skipped");
			return true;
		}

		uint32_t surfaceFormatCount = 0;
		uint32_t presentModeCount = 0;
		
		// Get supported surface format count
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &surfaceFormatCount, nullptr),
			"Failed to get physical device surface formats",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		// Get supported surface present mode count
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr),
			"Failed to get physical device surface present modes",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		const bool supported = surfaceFormatCount && presentModeCount;
		_context->logger->trace("Found {} supported surface format(s) and {} present mode(s) : The surface is {}", surfaceFormatCount, presentModeCount, supported ? "supported" : "unsopported");

		// The surface is supported if it supports at least one format and one present mode
		return supported;
	}

	bool PhysicalDevice::areExtensionsSupported(VkPhysicalDevice device){

		// Query supported extensions
		uint32_t count;
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr),
			"Failed to get physical device extension properties",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		std::vector<VkExtensionProperties> extensions(count);
		Exceptions::checkVkOperation<VkPhysicalDevice>(
			vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data()),
			"Failed to get physical device extension properties",
			Exceptions::VulkanOperationType::QUERYING,
			_context->logger
		);

		std::set<const char*> requiredSet(
			_requiredExtensions.begin(),
			_requiredExtensions.end()
		);

		// Remove the supported extensions from the required extensions
		for (const VkExtensionProperties& properties : extensions){
			requiredSet.erase(properties.extensionName);
		}

		// If the required extension set is empty, it means that they are all supported
		if (requiredSet.empty()){
			_context->logger->trace("Found every required extensions");
			return true;
		}
			
		_context->logger->trace("Missing extensions : ");
		for (auto& ext : requiredSet){
			_context->logger->trace("\t- {}", ext);
		}
		return false;
	}

	void PhysicalDevice::requireSurfaceSupport(VkSurfaceKHR surface){
		if (_device != VK_NULL_HANDLE){
			throw std::runtime_error("Cannot require surface support after initializing the physical device");
		}
		
		_surface = surface;
	}

	void PhysicalDevice::requireExtension(const char* ext){
		if (_device != VK_NULL_HANDLE){
			throw std::runtime_error("Cannot require extensions after initializing the physical device");
		}

		_requiredExtensions.push_back(ext);
	}

	void PhysicalDevice::queryProperties(){
		vkGetPhysicalDeviceProperties(_device, &_properties);
	}

	void PhysicalDevice::queryFeatures(){
		vkGetPhysicalDeviceFeatures(_device, &_features);
	}

	void PhysicalDevice::queryQueueProperties(){
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, nullptr);

		_queueProperties.resize(static_cast<std::size_t>(count));
		vkGetPhysicalDeviceQueueFamilyProperties(_device, &count, _queueProperties.data());
	}
	
	const VkPhysicalDeviceProperties& PhysicalDevice::getProperties() const noexcept{
		return _properties;
	}

	const VkPhysicalDeviceFeatures& PhysicalDevice::getFeatures() const noexcept{
		return _features;
	}

	const std::vector<VkQueueFamilyProperties>& PhysicalDevice::getQueueProperties() const noexcept{
		return _queueProperties;
	}
}