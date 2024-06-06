#ifndef __RAINDROP_INTERNAL_GRAPHICS_IMAGE_VIEW_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_IMAGE_VIEW_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct ImageViewConfigInfo : public VkImageViewCreateInfo{
		std::shared_ptr<Image> image;

		void update();
		ImageViewConfigInfo();
	};

	class ImageView{
		public:
			ImageView(Context& context, const ImageViewConfigInfo& info);
			~ImageView();

			VkImageView get() const;

			const VkImageViewCreateFlags& getFlags() const noexcept;
			const std::shared_ptr<Image>& getImage() const noexcept;
			const VkImageViewType& getViewType() const noexcept;
			const VkFormat& getFormat() const noexcept;
			const VkComponentMapping& getComponents() const noexcept;
			const VkImageSubresourceRange& getSubresourceRange() const noexcept;
			
		private:
			Context& _context;
			VkImageView _imageView;

			VkImageViewCreateFlags _flags;
			std::shared_ptr<Image> _image;
			VkImageViewType _viewType;
			VkFormat _format;
			VkComponentMapping _components;
			VkImageSubresourceRange _subresourceRange;
	};
}

#endif