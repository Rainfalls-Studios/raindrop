#ifndef __RAINDROP_GRAPHICS_BUILDERS_IMAGE_VIEW_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_BUILDERS_IMAGE_VIEW_BUILDER_HPP__

#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics::Builders{
	class ImageViewBuilder{
		public:
			ImageViewBuilder();
			~ImageViewBuilder();

			void setImage(const std::shared_ptr<Image>& image);
			std::shared_ptr<Image> image() const;

			void setViewType(VkImageViewType type);
			VkImageViewType viewType() const;

			void setFormat(VkFormat format);
			VkFormat format() const;

			void setComponentMapping(VkComponentMapping mapping);
			VkComponentMapping componentMapping() const;

			void setSubRessourceRange(VkImageSubresourceRange subRessource);
			VkImageSubresourceRange subRessourceRange() const;

		private:
			std::shared_ptr<Image> _image;
			VkImageViewType _viewType;
			VkFormat _format;
			VkComponentMapping _componentMapping;
			VkImageSubresourceRange _subRessource;
	};
}

#endif