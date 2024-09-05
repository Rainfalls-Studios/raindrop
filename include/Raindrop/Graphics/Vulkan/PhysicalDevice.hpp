#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;
	
	namespace Vulkan{
		class PhysicalDevice{
			public:
				PhysicalDevice(Context& context);
				~PhysicalDevice();

				PhysicalDevice(const PhysicalDevice&) = delete;
				PhysicalDevice& operator=(const PhysicalDevice&) = delete;

				const VkPhysicalDevice& get() const noexcept;

				vkb::PhysicalDevice& getVkb() noexcept;
				const vkb::PhysicalDevice& getVkb() const noexcept;

				std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

			private:
				Context& _context;
				vkb::PhysicalDevice _device;
		};
	}
}