// #include <Raindrop/Graphics/ImageView.hpp>
// #include <Raindrop/Graphics/GraphicsContext.hpp>
// #include <Raindrop/Graphics/Builders/ImageViewBuilder.hpp>
// #include <Raindrop/Graphics/Image.hpp>

// namespace Raindrop::Graphics{
// 	ImageView::ImageView(GraphicsContext& context, const Builders::ImageViewBuilder& builder) : _context{context}{
// 		_imageView = VK_NULL_HANDLE;

// 		_subRessource = builder.subRessourceRange();
// 		_viewType = builder.viewType();
// 		_format = builder.format();
// 		_componentMapping = builder.componentMapping();
// 		_image = builder.image();

// 		if (!_image){
// 			throw std::runtime_error("Cannot create an image view without a valid image");
// 		}

// 		VkImageViewCreateInfo info{};
// 		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
// 		info.image = _image->image();
// 		info.format = builder.format();
// 		info.subresourceRange = builder.subRessourceRange();
// 		info.viewType = builder.viewType();
// 		info.components = builder.componentMapping();

// 		auto device = context.device.get();
// 		auto allocationCallbacks = context.allocationCallbacks;

// 		if (vkCreateImageView(device, &info, allocationCallbacks, &_imageView) != VK_SUCCESS){
// 			throw std::runtime_error("Failed to create image view");
// 		}
// 	}

// 	ImageView::~ImageView(){
// 		auto device = _context.device().get();
// 		auto allocationCallbacks = _context.allocationCallbacks;

// 		if (_imageView) vkDestroyImageView(device, _imageView, allocationCallbacks);
// 		_imageView = VK_NULL_HANDLE;
// 	}

// 	const Image& ImageView::image() const{
// 		return *_image;
// 	}
	
// 	VkImageView ImageView::get() const{
// 		return _imageView;
// 	}
	
// 	VkImageSubresourceRange ImageView::subRessource() const{
// 		return _subRessource;
// 	}

// 	VkImageViewType ImageView::viewType() const{
// 		return _viewType;
// 	}

// 	VkFormat ImageView::format() const{
// 		return _format;
// 	}

// 	VkComponentMapping ImageView::componentMapping() const{
// 		return _componentMapping;
// 	}

// 	std::shared_ptr<ImageView> ImageView::create(GraphicsContext& context, const Builders::ImageViewBuilder& builder){
// 		return std::make_shared<ImageView>(context, builder);
// 	}
// }