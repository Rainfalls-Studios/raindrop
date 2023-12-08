#ifndef __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_IMAGE_VIEW_HPP__

#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Image{
	class ImageView{
		public:
			ImageView(Context& context, const ImageViewBuilder& builder);
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