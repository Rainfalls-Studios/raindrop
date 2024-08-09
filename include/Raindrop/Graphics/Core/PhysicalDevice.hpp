#ifndef __RAINDROP_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__
#define __RAINDROP_GRAPHICS_CORE_PHYSICAL_DEVICE_HPP__

#include "types.hpp"
#include "pch.pch"

namespace Raindrop::Graphics::Core{
	class PhysicalDevice{
		public:
			PhysicalDevice() noexcept;
			~PhysicalDevice();

			PhysicalDevice(const PhysicalDevice&) = delete;
			PhysicalDevice& operator=(const PhysicalDevice&) = delete;

			PhysicalDevice& prepare(Context& context);
			PhysicalDevice& initialize();
			void release();

			const VkPhysicalDevice& get() const noexcept;
			const vkb::PhysicalDevice& getVkb() const noexcept;

			PhysicalDevice& requireSurfaceSupport(VkSurfaceKHR surface);
			PhysicalDevice& requireExtension(const char* ext);
			PhysicalDevice& requireExtensions(const std::vector<const char*>& extensions);
			PhysicalDevice& requireFeatures(VkPhysicalDeviceFeatures& features);
			PhysicalDevice& require11Features(VkPhysicalDeviceVulkan11Features& features);
			PhysicalDevice& require12Features(VkPhysicalDeviceVulkan12Features& features);
			PhysicalDevice& require13Features(VkPhysicalDeviceVulkan13Features& features);

			std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

		private:
			Context* _context;
			vkb::PhysicalDevice _device;
			std::unique_ptr<vkb::PhysicalDeviceSelector> _selector;
	};
}

#endif