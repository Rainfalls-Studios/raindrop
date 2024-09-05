#pragma once

#include "Raindrop/pch.pch"

namespace Raindrop::Graphics{
	class Context;

	namespace Vulkan{
		class Surface{
			public:
				Surface(Context& context, VkSurfaceKHR surface) noexcept;
				~Surface();

				VkSurfaceKHR& get() noexcept;
				const VkSurfaceKHR& get() const noexcept;

			private:
				Context& _context;
				VkSurfaceKHR _surface = VK_NULL_HANDLE;
		};
	}
}