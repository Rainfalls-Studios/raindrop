#include <Raindrop_internal/Graphics/ImageView.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>

namespace Raindrop::Internal::Graphics{
	ImageViewConfigInfo::ImageViewConfigInfo() : 
		image{nullptr},
		::VkImageViewCreateInfo{
			.sType{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO},
			.pNext{nullptr},
			.flags{0},
			.image{VK_NULL_HANDLE},
			.viewType{VK_IMAGE_VIEW_TYPE_2D},
			.format{VK_FORMAT_UNDEFINED},
			.components{
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_A,
			},
			.subresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		}
	{}

	void ImageViewConfigInfo::update(){
		if (image){
			VkImageViewCreateInfo::image = image->get();
		}
	}

	ImageView::ImageView(Context& context, const ImageViewConfigInfo& info) :
		_context{context},
		_imageView{VK_NULL_HANDLE},

		_flags{info.flags},
		_image{info.image},
		_viewType{info.viewType},
		_format{info.format},
		_components{info.components},
		_subresourceRange{info.subresourceRange}
	{
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		Exceptions::checkVkCreation<VkImageView>(
			vkCreateImageView(device.get(), &info, allocationCallbacks, &_imageView),
			"Failed to create image view"
		);
	}

	ImageView::~ImageView(){
		auto& device = _context.getDevice();
		auto& allocationCallbacks = _context.getAllocationCallbacks();

		if (_imageView){
			vkDestroyImageView(device.get(), _imageView, allocationCallbacks);
			_imageView = VK_NULL_HANDLE;
		}
	}
	
	VkImageView ImageView::get() const{
		return _imageView;
	}
	
	const VkImageViewCreateFlags& ImageView::getFlags() const noexcept{
		return _flags;
	}

	const std::shared_ptr<Image>& ImageView::getImage() const noexcept{
		return _image;
	}

	const VkImageViewType& ImageView::getViewType() const noexcept{
		return _viewType;
	}

	const VkFormat& ImageView::getFormat() const noexcept{
		return _format;
	}

	const VkComponentMapping& ImageView::getComponents() const noexcept{
		return _components;
	}

	const VkImageSubresourceRange& ImageView::getSubresourceRange() const noexcept{
		return _subresourceRange;
	}
}