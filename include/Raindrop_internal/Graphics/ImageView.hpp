#ifndef __RAINDROP_INTERNAL_GRAPHICS_IMAGE_VIEW_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_IMAGE_VIEW_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct ImageViewConfigInfo{
		std::shared_ptr<Image> image;
		VkImageViewType viewType;
		VkFormat format;
		VkComponentMapping componentMapping;
		VkImageSubresourceRange subResource;

		ImageViewConfigInfo();
	};

	class ImageView{
		public:
			ImageView(Context& context, const ImageViewConfigInfo& configInfo);
			~ImageView();

			VkImageView get() const;
			const Image& image() const;
			VkImageSubresourceRange subResource() const;
			VkImageViewType viewType() const;
			VkFormat format() const;
			VkComponentMapping componentMapping() const;
			
		private:
			Context& _context;
			std::shared_ptr<Image> _image;

			VkImageView _imageView;
			VkImageSubresourceRange _subResource;
			VkImageViewType _viewType;
			VkFormat _format;
			VkComponentMapping _componentMapping;
	};
}

#endif