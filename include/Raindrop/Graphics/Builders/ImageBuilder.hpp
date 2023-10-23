#ifndef __RAINDROP_GRAPHICS_BUILDERS_IMAGE_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_IMAGE_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/FormatUtilities.hpp>

namespace Raindrop::Graphics::Builders{
	class ImageBuilder{
		public:
			ImageBuilder();
			~ImageBuilder();

			void setFormat(VkFormat format);
			VkFormat format() const;

			void setExtent(uint32_t w, uint32_t h = 1, uint32_t d = 1);
			VkExtent3D extent() const;

			void setType(VkImageType type);
			VkImageType type() const;

			void setUsage(VkImageUsageFlags usage);
			VkImageUsageFlags usage() const;

			void setlayout(VkImageLayout layout);
			VkImageLayout layout() const;

			void setMipLevels(uint32_t levels);
			uint32_t mipLevels() const;

			void setArrayLayers(uint32_t layers);
			uint32_t arrayLayers() const;

			void setSampleCount(VkSampleCountFlagBits sample);
			VkSampleCountFlagBits sampleCount() const;

			void setTiling(VkImageTiling tiling);
			VkImageTiling tiling() const;

			void addQueueFamily(uint32_t queueFamilyIndex);
			const uint32_t* queueFamilyIndices() const;
			uint32_t queueFamilyCount() const;
			VkSharingMode sharingMode() const;

		private:
			VkFormat _format;
			VkExtent3D _extent;
			VkImageUsageFlags _usage;
			VkImageLayout _layout;
			uint32_t _mipLevels;
			uint32_t _arrLayers;
			VkSampleCountFlagBits _sampleCount;
			VkImageTiling _tiling;
			std::vector<uint32_t> _queueFamilies;
			VkImageType _type;

	};
}

#endif