#pragma once

#include "../pch.pch"

namespace Raindrop::Graphics::Vulkan::Core{
	class Context;
	
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
			vkb::PhysicalDevice& getVkb() noexcept;
			const vkb::PhysicalDevice& getVkb() const noexcept;

			PhysicalDevice& requireSurfaceSupport(VkSurfaceKHR surface);
			PhysicalDevice& requireExtension(const char* ext);
			PhysicalDevice& requireExtensions(const std::vector<const char*>& extensions);
			PhysicalDevice& requireFeatures(const VkPhysicalDeviceFeatures& features);
			// PhysicalDevice& require11Features(const VkPhysicalDeviceVulkan11Features& features);
			// PhysicalDevice& require12Features(const VkPhysicalDeviceVulkan12Features& features);
			// PhysicalDevice& require13Features(const VkPhysicalDeviceVulkan13Features& features);

			std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

			vkb::PhysicalDeviceSelector& getSelector();

		private:
			Context* _context;
			vkb::PhysicalDevice _device;
			std::unique_ptr<vkb::PhysicalDeviceSelector> _selector;
	};
}