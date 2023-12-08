#ifndef __RAINDROP_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class Image{
		public:
			Image(Context& context, const ImageBuilder& builder);
			~Image();

			VkImage image() const;
			VkFormat format() const;
			VkExtent3D extent() const;
			uint32_t mipLevels() const;
			uint32_t arrayLayers() const;

		private:
			Context& _context;

			VkImage _image;
			VkFormat _format;
			VkDeviceMemory _memory;
			VkExtent3D _extent;
			uint32_t _mipLevels;
			uint32_t _arrLayers;
	};
}

#endif