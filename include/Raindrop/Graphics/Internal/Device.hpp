#ifndef __RAINDROP_GRAPHICS_INTERNAL_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_INTERNAL_DEVICE_HPP__

#include <Raindrop/Graphics/Internal/common.hpp>
#include <Raindrop/Graphics/Internal/PhysicalDevice.hpp>

namespace Raindrop::Graphics::Internal{
	class Device{
		public:
			static constexpr std::array<const char*, 1> REQUIRED_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			static constexpr std::array<const char*, 0> REQUIRED_LAYERS = {};

			Device(Context& context);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			VkDevice get() const;

			std::vector<VkQueueFamilyProperties> getQueueFamilyProperties();
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

			void waitIdle();

		private:
			Context& _context;
			VkDevice _device;

			VkPhysicalDeviceProperties _properties;

			void findPhysicalDevice();
			bool isPhysicalDeviceSuitable(VkPhysicalDevice device);
			void build();

			bool isExtensionsSupported();
			bool isLayersSupported();

			bool isExtensionSupported(const char* extensionName);
			bool isLayerSupported(const char* layerName);

			uint32_t getGraphicsFamily();
			uint32_t getTransfertFamily();
			uint32_t getPresentFamily();

	};
}

#endif