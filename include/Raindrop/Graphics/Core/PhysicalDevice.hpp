#ifndef __RAINDROP_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__

#include "common.hpp"
#include <Raindrop/Graphics/pch.pch>

namespace Raindrop::Graphics::Core{
	class PhysicalDevice{
		public:
			PhysicalDevice() noexcept;
			~PhysicalDevice();

			PhysicalDevice(const PhysicalDevice&) = delete;
			PhysicalDevice& operator=(const PhysicalDevice&) = delete;

			void initialize(Context& context);
			void release();

			VkPhysicalDevice get() const noexcept;

			const VkPhysicalDeviceProperties& getProperties() const noexcept;
			const VkPhysicalDeviceFeatures& getFeatures() const noexcept;
			const std::vector<VkQueueFamilyProperties>& getQueueProperties() const noexcept;

			void requireSurfaceSupport(VkSurfaceKHR surface);
			void requireExtension(const char* ext);

		private:
			Context* _context;
			VkPhysicalDevice _device;

			VkSurfaceKHR _surface;
			std::list<const char*> _requiredExtensions;

			VkPhysicalDeviceProperties _properties;
			VkPhysicalDeviceFeatures _features;
			std::vector<VkQueueFamilyProperties> _queueProperties;

			void findPhysicalDevice();
			bool isDeviceSuitable(VkPhysicalDevice device);

			bool isSurfaceSupported(VkPhysicalDevice device);
			bool areExtensionsSupported(VkPhysicalDevice device);

			void queryProperties();
			void queryFeatures();
			void queryQueueProperties();
	};
}

#endif