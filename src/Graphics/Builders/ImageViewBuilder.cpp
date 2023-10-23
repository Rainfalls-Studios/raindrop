#include <Raindrop/Graphics/Builders/ImageViewBuilder.hpp>
#include <Raindrop/Graphics/Image.hpp>

namespace Raindrop::Graphics::Builders{
	ImageViewBuilder::ImageViewBuilder(){
		_image = nullptr;
		_viewType = VK_IMAGE_VIEW_TYPE_2D;
		_format = VK_FORMAT_UNDEFINED;

		_componentMapping = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		};

	}

	ImageViewBuilder::~ImageViewBuilder(){

	}

	void ImageViewBuilder::setImage(const std::shared_ptr<Image>& image){
		_image = image;
		_format = image ? image->format() : VK_FORMAT_UNDEFINED;
	}

	std::shared_ptr<Image> ImageViewBuilder::image() const{
		return _image;
	}

	void ImageViewBuilder::setViewType(VkImageViewType type){
		_viewType = type;
	}

	VkImageViewType ImageViewBuilder::viewType() const{
		return _viewType;
	}

	void ImageViewBuilder::setFormat(VkFormat format){
		_format = format;
	}

	VkFormat ImageViewBuilder::format() const{
		return _format;
	}

	void ImageViewBuilder::setComponentMapping(VkComponentMapping mapping){
		_componentMapping = mapping;
	}

	VkComponentMapping ImageViewBuilder::componentMapping() const{
		return _componentMapping;
	}

	void ImageViewBuilder::setSubRessourceRange(VkImageSubresourceRange subRessource){
		_subRessource = subRessource;
	}

	VkImageSubresourceRange ImageViewBuilder::subRessourceRange() const{
		return _subRessource;
	}
}