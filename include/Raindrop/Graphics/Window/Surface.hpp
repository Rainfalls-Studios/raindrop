#ifndef __RAINDROP_GRAPHICS_WINDOW_SURFACE_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_SURFACE_HPP__

#include "types.hpp"
#include "pch.pch"
#include <Raindrop/Graphics/Core/types.hpp>

namespace Raindrop::Graphics::Window{
	class Surface{
		public:
			Surface() noexcept;
			~Surface();

			Surface& prepare(Context& context, Core::Context& core);
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