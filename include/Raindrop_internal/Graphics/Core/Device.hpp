#ifndef __RAINDROP_INTERNAL_GRAPHICS_CORE_DEVICE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_CORE_DEVICE_HPP__

#include "common.hpp"
#include "PhysicalDevice.hpp"

namespace Raindrop::Internal::Graphics::Core{
	class Device{
		public:
			static constexpr const char* REQUIRED_EXTENSIONS[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			static constexpr const char* REQUIRED_LAYERS[] = {};

			static constexpr std::size_t REQUIRED_EXTENSIONS_COUNT = sizeof(REQUIRED_EXTENSIONS) / sizeof(const char*);
			static constexpr std::size_t REQUIRED_LAYERS_COUNT = sizeof(REQUIRED_LAYERS) / sizeof(const char*);

			Device(Context& context);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			VkDevice get() const;

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

	};
}

#endif