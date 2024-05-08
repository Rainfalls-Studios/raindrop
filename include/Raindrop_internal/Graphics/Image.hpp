#ifndef __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_IMAGE_HPP__

#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>

namespace Raindrop::Internal::Graphics{
	struct ImageConfigInfo{
		VkFormat format;
		VkExtent3D extent;
		VkImageUsageFlags usage;
		VkImageLayout layout;
		uint32_t mipLevels;
		uint32_t arrLayers;
		VkSampleCountFlagBits sampleCount;
		VkImageTiling tiling;
		VkImageType type;
		VkFormatFeatureFlags features;
		VkImageCreateFlags flags;
		VkSharingMode sharingMode;
		
		std::vector<uint32_t> queueFamilies;

		ImageConfigInfo();
	};

	class Image{
		public:
			Image(Context& context, const ImageConfigInfo& config);
			~Image();

			VkImage get() const;
			VkDeviceMemory memory() const;
			VkFormat format() const;
			VkExtent3D extent() const;

		private:
			Context& _context;

			VkImage _image;
			VkFormat _format;
			VkDeviceMemory _memory;
			VkExtent3D _extent;
	};
}

#endif