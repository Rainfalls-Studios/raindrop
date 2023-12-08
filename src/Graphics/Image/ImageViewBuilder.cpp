#include <Raindrop/Graphics/Image/ImageViewBuilder.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>
#include <Raindrop/Graphics/Image/Image.hpp>

namespace Raindrop::Graphics::Image{
	ImageViewBuilder::ImageViewBuilder(Context& context) : _context{context}{
		_image = nullptr;
		_viewType = VK_IMAGE_VIEW_TYPE_2D;
		_format = VK_FORMAT_UNDEFINED;
		_name = "";

		_componentMapping = {
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		};
	}

	ImageViewBuilder::~ImageViewBuilder(){}

	ImageViewBuilder& ImageViewBuilder::operator=(const ImageViewBuilder& other){
		_image = other._image;
		_viewType = other._viewType;
		_format = other._format;
		_componentMapping = other._componentMapping;
		_subResource = other._subResource;
		_name = other._name;

		return *this;
	}

	void ImageViewBuilder::loadFromNode(const YAML::Node& node){
		_name = node["name"].as<std::string>();
		_image = _context.imageManager().get(node["base"].as<std::string>());
		_viewType = node["viewType"].as<VkImageViewType>(VK_IMAGE_VIEW_TYPE_2D);
	
		{
			auto& formatNode = node["format"];

			if (formatNode.IsNull()){
				_format = _image->format();
			} else {
				_format = formatNode.as<VkFormat>(_image->format());
			}
		}
		
		{
			auto& componentMapping = node["componentMapping"];

			_componentMapping.a = componentMapping["a"].as<VkComponentSwizzle>(VK_COMPONENT_SWIZZLE_IDENTITY);
			_componentMapping.r = componentMapping["r"].as<VkComponentSwizzle>(VK_COMPONENT_SWIZZLE_IDENTITY);
			_componentMapping.g = componentMapping["g"].as<VkComponentSwizzle>(VK_COMPONENT_SWIZZLE_IDENTITY);
			_componentMapping.b = componentMapping["b"].as<VkComponentSwizzle>(VK_COMPONENT_SWIZZLE_IDENTITY);
		}

		{
			auto& subResource = node["subresourceRange"];
			YAML::decodeVkImageAspectFlags(subResource["aspectMask"], _subResource.aspectMask);
			_subResource.baseArrayLayer = subResource["baseArrayLayer"].as<uint32_t>(0);
			_subResource.baseMipLevel = subResource["baseMipLevel"].as<uint32_t>(0);
			_subResource.layerCount = subResource["layerCount"].as<uint32_t>(1);
			_subResource.levelCount = subResource["levelCount"].as<uint32_t>(1);
		}
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

	void ImageViewBuilder::setSubResourceRange(VkImageSubresourceRange subResource){
		_subResource = subResource;
	}

	VkImageSubresourceRange ImageViewBuilder::subResourceRange() const{
		return _subResource;
	}

	std::string& ImageViewBuilder::name(){
		return _name;
	}

	const std::string& ImageViewBuilder::name() const{
		return _name;
	}

	void ImageViewBuilder::setName(const std::string& name){
		_name = name;
	}
}