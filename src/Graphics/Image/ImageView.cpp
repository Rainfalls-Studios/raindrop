#include <Raindrop/Graphics/Image/ImageView.hpp>
#include <Raindrop/Graphics/Image/ImageViewBuilder.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>
#include <Raindrop/Graphics/Image/Image.hpp>

namespace Raindrop::Graphics::Image{
	ImageView::ImageView(Context& context, const ImageViewBuilder& builder) : _context{context}{
		_imageView = VK_NULL_HANDLE;

		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		_subResource = builder.subResourceRange();
		_viewType = builder.viewType();
		_format = builder.format();
		_componentMapping = builder.componentMapping();
		_image = builder.image();

		if (!_image){
			_context.logger().error("Cannot create an image view without a valid image");
			throw std::runtime_error("Cannot create an image view without a valid image");
		}

		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _image->image();
		info.format = builder.format();
		info.subresourceRange = builder.subResourceRange();
		info.viewType = builder.viewType();
		info.components = builder.componentMapping();

		if (vkCreateImageView(device.get(), &info, allocationCallbacks, &_imageView) != VK_SUCCESS){
			_context.logger().error("Failed to create vulkan image view");
			throw std::runtime_error("Failed to create image view");
		}
	}

	ImageView::~ImageView(){
		auto& device = _context.graphics().internal().device();
		auto& allocationCallbacks = _context.graphics().allocationCallbacks();

		if (_imageView) vkDestroyImageView(device.get(), _imageView, allocationCallbacks);
		_imageView = VK_NULL_HANDLE;
	}

	const Image& ImageView::image() const{
		return *_image;
	}
	
	VkImageView ImageView::get() const{
		return _imageView;
	}
	
	VkImageSubresourceRange ImageView::subResource() const{
		return _subResource;
	}

	VkImageViewType ImageView::viewType() const{
		return _viewType;
	}

	VkFormat ImageView::format() const{
		return _format;
	}

	VkComponentMapping ImageView::componentMapping() const{
		return _componentMapping;
	}
}