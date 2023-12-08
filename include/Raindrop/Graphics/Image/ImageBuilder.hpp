#ifndef __RAINDROP_GRAPHICS_IMAGE_IMAGE_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_IMAGE_BUILDER_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class ImageBuilder{
		public:
			ImageBuilder(Context& context);
			~ImageBuilder();

			ImageBuilder& operator=(const ImageBuilder& other);

			void loadFromNode(const YAML::Node& node);

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

			std::string& name();
			const std::string& name() const;
			void setName(const std::string& name);

			VkImageCreateFlags flags() const;
			void setFlags(VkImageCreateFlags flags);

		private:
			Context& _context;

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
			std::string _name;
			VkFormatFeatureFlags _features;
			VkImageCreateFlags _flags;

	};
}

#endif