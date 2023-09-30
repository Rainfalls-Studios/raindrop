#ifndef __RAINDROP_GRAPHICS_IMAGE_VIEW_HPP__
#define __RAINDROP_GRAPHICS_IMAGE_VIEW_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics{
	class ImageView{
		public:
			static std::shared_ptr<ImageView> create(GraphicsContext& context, const Builders::ImageViewBuilder& builder);
			ImageView(GraphicsContext& context, const Builders::ImageViewBuilder& builder);
			~ImageView();

			VkImageView get() const;
			const Image& image() const;
			VkImageSubresourceRange subRessource() const;
			VkImageViewType viewType() const;
			VkFormat format() const;
			VkComponentMapping componentMapping() const;
			
		private:
			GraphicsContext& _context;
			std::shared_ptr<Image> _image;

			VkImageView _imageView;
			VkImageSubresourceRange _subRessource;
			VkImageViewType _viewType;
			VkFormat _format;
			VkComponentMapping _componentMapping;
	};
}

#endif