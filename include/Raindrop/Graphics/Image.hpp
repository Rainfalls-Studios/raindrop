#ifndef __RAINDROP_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics{
	class Image{
		public:
			static std::shared_ptr<Image> create(GraphicsContext& context, const Builders::ImageBuilder& builder);
			Image(GraphicsContext& context, const Builders::ImageBuilder& builder);
			~Image();

			VkImage image() const;
			VkFormat format() const;
			VkExtent3D extent() const;
			uint32_t mipLevels() const;
			uint32_t arrayLayers() const;

		private:
			GraphicsContext& _context;

			VkImage _image;
			VkFormat _format;
			VkDeviceMemory _memory;
			VkExtent3D _extent;
			uint32_t _mipLevels;
			uint32_t _arrLayers;
	};
}

#endif