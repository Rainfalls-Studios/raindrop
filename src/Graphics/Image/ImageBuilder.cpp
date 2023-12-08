#include <Raindrop/Graphics/Image/ImageBuilder.hpp>
#include <Raindrop/Graphics/Image/Context.hpp>
#include <Raindrop/Core/Engine.hpp>

namespace Raindrop::Graphics::Image{
	ImageBuilder::ImageBuilder(Context& context) : _context{context}{
		_format = VK_FORMAT_UNDEFINED;
		_extent = VkExtent3D{255, 255, 1};
		_usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		_layout = VK_IMAGE_LAYOUT_GENERAL;
		_mipLevels = 1;
		_arrLayers = 1;
		_sampleCount = VK_SAMPLE_COUNT_1_BIT;
		_tiling = VK_IMAGE_TILING_OPTIMAL;
		_flags = 0;
		_name = "";
	}

	ImageBuilder::~ImageBuilder(){}

	ImageBuilder& ImageBuilder::operator=(const ImageBuilder& other){
		_format = other._format;
		_extent = other._extent;
		_usage = other._usage;
		_layout = other._layout;
		_mipLevels = other._mipLevels;
		_arrLayers = other._arrLayers;
		_sampleCount = other._sampleCount;
		_tiling = other._tiling;
		_queueFamilies = other._queueFamilies;
		_type = other._type;
		_name = other._name;
		_flags = other._flags;

		return *this;
	}

	void ImageBuilder::loadFromNode(const YAML::Node& node){
		auto& registry = _context.graphics().core().registry();

		_name = node["name"].as<std::string>();
		_context.logger().info("Loading \"{}\" image", _name);
		
		{
			const YAML::Node& extentNode = node["extent"];
			_extent.width = registry.getFromNode<uint32_t>(extentNode["width"], "1");
			_extent.height = registry.getFromNode<uint32_t>(extentNode["height"], "1");
			_extent.depth = registry.getFromNode<uint32_t>(extentNode["depth"], "1");
		}

		_type = node["type"].as<VkImageType>(VK_IMAGE_TYPE_2D);
		_layout = node["initialLayout"].as<VkImageLayout>(VK_IMAGE_LAYOUT_GENERAL);
		YAML::decodeVkImageUsageFlags(node["usage"], _usage);
		_mipLevels = registry.get<uint32_t>(node["mipLevels"].as<std::string>("1"));
		_arrLayers = registry.get<uint32_t>(node["arrayLayers"].as<std::string>("1"));
		_sampleCount = node["sampleCount"].as<VkSampleCountFlagBits>(VK_SAMPLE_COUNT_1_BIT);
		_tiling = node["tiling"].as<VkImageTiling>(VK_IMAGE_TILING_OPTIMAL);
		YAML::decodeVkImageCreateFlags(node["flags"], _flags);
		YAML::decodeVkFormatFeatureFlags(node["features"], _features);

		{
			const YAML::Node& familiesNode = node["queueFamilies"];
			for (const auto& familyQueue : familiesNode){	
				uint32_t familyIndex = registry.get<uint32_t>(familyQueue.as<std::string>());
				_queueFamilies.push_back(familyIndex);
			}
		}
		
		_format = _context.graphics().formats().registry().get(node["format"].as<std::string>());
	}

	void ImageBuilder::setFormat(VkFormat format){
		_format = format;
	}

	VkFormat ImageBuilder::format() const{
		return _format;
	}
	
	void ImageBuilder::setExtent(uint32_t w, uint32_t h, uint32_t d){
		auto& maxImageArrayLayers = _context.graphics().internal().limits().maxImageArrayLayers;
		if (d > maxImageArrayLayers){
			_context.logger().warn("Image layer count out of bounds {} > {}", d, maxImageArrayLayers);
			d = maxImageArrayLayers;
		}
		_extent = VkExtent3D{w, h, d};

	}
	
	void ImageBuilder::setType(VkImageType type){
		_type = type;
	}

	VkImageType ImageBuilder::type() const{
		return _type;
	}

	VkExtent3D ImageBuilder::extent() const{
		return _extent;
	}

	void ImageBuilder::setUsage(VkImageUsageFlags usage){
		_usage = usage;
	}

	VkImageUsageFlags ImageBuilder::usage() const{
		return _usage;
	}

	void ImageBuilder::setlayout(VkImageLayout layout){
		_layout = layout;
	}

	VkImageLayout ImageBuilder::layout() const{
		return _layout;
	}

	void ImageBuilder::setMipLevels(uint32_t levels){
		_mipLevels = levels;
	}
	
	uint32_t ImageBuilder::mipLevels() const{
		return _mipLevels;
	}

	void ImageBuilder::setArrayLayers(uint32_t layers){
		_arrLayers = layers;
	}

	uint32_t ImageBuilder::arrayLayers() const{
		return _arrLayers;
	}
	
	void ImageBuilder::setSampleCount(VkSampleCountFlagBits sample){
		_sampleCount = sample;
	}

	VkSampleCountFlagBits ImageBuilder::sampleCount() const{
		return _sampleCount;
	}

	void ImageBuilder::setTiling(VkImageTiling tiling){
		_tiling = tiling;
	}

	VkImageTiling ImageBuilder::tiling() const{
		return _tiling;
	}
	
	void ImageBuilder::addQueueFamily(uint32_t queueFamilyIndex){
		_queueFamilies.push_back(queueFamilyIndex);
	}

	const uint32_t* ImageBuilder::queueFamilyIndices() const{
		return _queueFamilies.data();
	}

	uint32_t ImageBuilder::queueFamilyCount() const{
		return _queueFamilies.size();
	}

	VkSharingMode ImageBuilder::sharingMode() const{
		uint32_t tempIndex = static_cast<uint32_t>(-1);
		for (const auto& family : _queueFamilies){
			if (tempIndex == static_cast<uint32_t>(-1)){
				tempIndex = family;
				continue;
			}

			if (family != tempIndex){
				return VK_SHARING_MODE_CONCURRENT;
			}
		}
		return VK_SHARING_MODE_EXCLUSIVE;
	}

	std::string& ImageBuilder::name(){
		return _name;
	}

	const std::string& ImageBuilder::name() const{
		return _name;
	}

	void ImageBuilder::setName(const std::string& name){
		_name = name;
	}


	VkImageCreateFlags ImageBuilder::flags() const{
		return _flags;
	}

	void ImageBuilder::setFlags(VkImageCreateFlags flags){
		_flags = flags;
	}
}