#include <Raindrop/Graphics/ImageView.hpp>
#include <Raindrop/Graphics/Image.hpp>
#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Graphics{
	ImageView::ImageView(Context& context, const ImageViewConfigInfo& builder) :
		_context{context},
		_image{nullptr},
		_imageView{VK_NULL_HANDLE},
		_subResource{},
		_format{VK_FORMAT_UNDEFINED},
		_componentMapping{}
	{
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		_subResource = builder.subResource;
		_viewType = builder.viewType;
		_format = builder.format;
		_componentMapping = builder.componentMapping;
		_image = builder.image;

		assert(_image);

		VkImageViewCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.image = _image->get();
		info.format = _format;
		info.subresourceRange = builder.subResource;
		info.viewType = _viewType;
		info.components = _componentMapping;

		Exceptions::checkVkCreation<VkImageView>(
			vkCreateImageView(device.get(), &info, allocationCallbacks, &_imageView),
			"Failed to create image view"
		);
	}

	ImageView::~ImageView(){
		auto& device = _context.device;
		auto& allocationCallbacks = _context.allocationCallbacks;

		if (_imageView){
			vkDestroyImageView(device.get(), _imageView, allocationCallbacks);
			_imageView = VK_NULL_HANDLE;
		}
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