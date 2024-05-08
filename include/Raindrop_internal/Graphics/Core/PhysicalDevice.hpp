#ifndef __RAINDROP_INTERNAL_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__

#include "common.hpp"
#include "SwapchainSupport.hpp"

namespace Raindrop::Internal::Graphics::Core{
	class PhysicalDevice{
		public:
			PhysicalDevice(Context& context, VkPhysicalDevice device = VK_NULL_HANDLE);

			PhysicalDevice& operator=(VkPhysicalDevice device);

			uint32_t apiVersion() const;
			uint32_t driverVersion() const;
			uint32_t vendorID() const;
			uint32_t deviceID() const;
			VkPhysicalDeviceType type() const;
			const char* name() const;

			const VkPhysicalDeviceSparseProperties& sparseProperties() const;
			const VkPhysicalDeviceFeatures& features() const;
			const VkPhysicalDeviceProperties& properties() const;
			const VkPhysicalDeviceMemoryProperties& memoryProperties() const;
			const VkPhysicalDeviceLimits& limits() const;

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
			Context& _context;
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
			void getlimits();

			void reset();
	};
}

#endif