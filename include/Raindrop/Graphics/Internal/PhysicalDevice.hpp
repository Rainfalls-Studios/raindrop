#ifndef __RAINDROP_GRAPHICS_INTERNAL_PHYSICAL_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_PHYSICAL_DEVICE_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>

namespace Raindrop::Graphics::Internal{
	class PhysicalDevice{
		public:
			PhysicalDevice(VkPhysicalDevice device = VK_NULL_HANDLE);

			PhysicalDevice& operator=(VkPhysicalDevice device);

			const VkPhysicalDeviceFeatures& features() const;
			const VkPhysicalDeviceProperties& properties() const;
			const VkPhysicalDeviceMemoryProperties& memoryProperties() const;

			const std::vector<VkPhysicalDeviceToolProperties>& toolProperties() const;
			const std::vector<VkQueueFamilyProperties>& queueFamilyProperties() const;
			const std::vector<VkExtensionProperties>& getSupportedExtensionProperties() const;
			const std::vector<VkLayerProperties>& getSupportedLayerProperties() const;

			bool areFeaturesSupported(const VkPhysicalDeviceFeatures& features) const;
			bool isExtensionSupported(const char* extension) const;
			bool isLayerSupported(const char* layer) const;

			SwapchainSupport getSwapchainSupport(VkSurfaceKHR surface) const;
			VkPhysicalDevice get() const;

		private:
			VkPhysicalDevice _device;

			VkPhysicalDeviceFeatures _features;
			VkPhysicalDeviceProperties _properties;
			VkPhysicalDeviceMemoryProperties _memoryProperties;

			std::vector<VkPhysicalDeviceToolProperties> _toolProperties;
			std::vector<VkQueueFamilyProperties> _queueFamilyProperties;
			std::vector<VkExtensionProperties> _supportedExtensionProperties;
			std::vector<VkLayerProperties> _supportedLayerProperties;
			
			void getFeatures();
			void getProperties();
			void getMemoryProperties();
			void getToolProperties();
			void getFamilyProperties();
			void getExtensionProperties();
			void getLayerProperties();

			void reset();
	};
}

#endif