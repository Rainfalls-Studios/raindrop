#include <Raindrop/Graphics/Builders/ImageBuilder.hpp>
#include <Raindrop/Graphics/GraphicsContext.hpp>

namespace Raindrop::Graphics::Builders{
	ImageBuilder::ImageBuilder(){
		_format = VK_FORMAT_R8G8B8A8_UINT;
		_extent = VkExtent3D{255, 255, 1};
		_usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		_layout = VK_IMAGE_LAYOUT_GENERAL;
		_mipLevels = 1;
		_arrLayers = 1;
		_sampleCount = VK_SAMPLE_COUNT_1_BIT;
		_tiling = VK_IMAGE_TILING_OPTIMAL;
	}

	ImageBuilder::~ImageBuilder(){}

	void ImageBuilder::setFormat(VkFormat format){
		_format = format;
	}

	VkFormat ImageBuilder::format() const{
		return _format;
	}
	
	void ImageBuilder::setExtent(uint32_t w, uint32_t h, uint32_t d){
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
}