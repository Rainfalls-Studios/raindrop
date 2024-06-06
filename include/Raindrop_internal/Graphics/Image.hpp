#ifndef __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__

#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

namespace Raindrop::Internal::Graphics{
	class Image{
		public:
			Image(Context& context, const VkImageCreateInfo& info);
			~Image();

			VkImage get() const;
			VkDeviceMemory memory() const;

		private:
			Context& _context;

			VkImage _image;
			VkDeviceMemory _memory;
	};
}

#endif