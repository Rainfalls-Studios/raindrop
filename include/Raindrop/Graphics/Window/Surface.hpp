#ifndef __RAINDROP_GRAPHICS_WINDOW_SURFACE_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_SURFACE_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/Core/prototypes.hpp>
#include <vulkan/vulkan.h>
#include <VkBootstrap.h>

namespace Raindrop::Graphics::Window{
	class Surface{
		public:
			Surface() noexcept;
			~Surface();

			void prepare(Context& context, Core::Context& core);
			void initialize();
			void release();

			const VkSurfaceKHR& get() const noexcept; 

		private:
			Context* _context;
			Core::Context* _core;
			VkSurfaceKHR _surface;
	};
}

#endif