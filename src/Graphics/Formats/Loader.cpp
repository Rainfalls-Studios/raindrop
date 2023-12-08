#include <Raindrop/Graphics/Formats/Loader.hpp>
#include <Raindrop/Graphics/Formats/Context.hpp>
#include <Raindrop/Graphics/Utils/StringToVulkan.hpp>

namespace Raindrop::Graphics::Formats{
	Loader::Loader(Context& context) : _context{context}{}
	Loader::~Loader(){}

	void Loader::loadFormats(const YAML::Node& node){
		_context.logger().info("Loading {} formats...", node.size());
		for (const auto& child : node){
			loadFormat(child);
		}
	}

	void Loader::loadFormat(const YAML::Node& node){
		auto& physicalDevice = _context.graphics().internal().physicalDevice();
		YAML::Mark mark = node.Mark();

		std::string name = node["name"].as<std::string>();

		VkFormatProperties requiredFormatProperties;
		if (const auto& features = node["features"]){
			getFeatures(features, requiredFormatProperties);
		}

		bool isImage = false;
		ImageRequirements requiredImageProperties;
		if (const auto& image = node["image"]){
			getImageRequirements(image, requiredImageProperties);
			isImage = true;
		}

		std::list<VkFormat> formats;
		getCandidates(node["candidates"], formats);

		VkFormat finalFormat = VK_FORMAT_UNDEFINED;

		for (const auto& format : formats){
			const char* formatStr = Utils::StringToVulkan::VkFormatToStr(format);
			_context.logger().trace("Trying \"{}\" format compatibility...", formatStr);
			
			if (isImage){
				const auto &requirements = requiredImageProperties;
				VkImageFormatProperties properties;
				VkResult result = vkGetPhysicalDeviceImageFormatProperties(physicalDevice.get(), format, requirements.type, requirements.tiling, requirements.usage, requirements.flags, &properties);

				if (result == VK_ERROR_FORMAT_NOT_SUPPORTED){
					_context.logger().trace("Image format \"{}\" support failed : format not supported", formatStr);
					continue;
				}

				{
					const auto& requiredProperties = requirements.properties;
					if (requiredProperties.maxArrayLayers > properties.maxArrayLayers){
						_context.logger().trace("Image format \"{}\" support failed : max array layers not supported", formatStr);
						continue;
					}

					if ((requiredProperties.sampleCounts & properties.sampleCounts) != requirements.properties.sampleCounts){
						_context.logger().trace("Image format \"{}\" support failed : required sample count(s) not present(s)");
						continue;
					}

					if (requiredProperties.maxMipLevels > properties.maxMipLevels){
						_context.logger().trace("Image format \"{}\" support failed : required mipmap level not supported", formatStr);
						continue;
					}
				}

				{
					const auto& requiredExtent = requirements.properties.maxExtent;
					const auto& supportedExtent = properties.maxExtent;

					if (requiredExtent.width > supportedExtent.width){
						_context.logger().trace("Image format \"{}\" support failed : required extent width not supported", formatStr);
						continue;
					}

					if (requiredExtent.height > supportedExtent.height){
						_context.logger().trace("Image format \"{}\" support failed : required extent height not supported", formatStr);
						continue;
					}

					if (requiredExtent.depth > supportedExtent.depth){
						_context.logger().trace("Image format \"{}\" support failed : required extent depth not supported", formatStr);
						continue;
					}
				}
			}

			{
				VkFormatProperties formatProperties;
				vkGetPhysicalDeviceFormatProperties(physicalDevice.get(), format, &formatProperties);

				if ((requiredFormatProperties.bufferFeatures & requiredFormatProperties.bufferFeatures) != requiredFormatProperties.bufferFeatures){
					_context.logger().trace("Format \"{}\" suppord failed : required buffer features not supported", formatStr);
					continue;
				}

				if ((requiredFormatProperties.linearTilingFeatures & requiredFormatProperties.linearTilingFeatures) != requiredFormatProperties.linearTilingFeatures){
					_context.logger().trace("Format \"{}\" suppord failed : required linear tiling features not supported", formatStr);
					continue;
				}

				if ((requiredFormatProperties.optimalTilingFeatures & requiredFormatProperties.optimalTilingFeatures) != requiredFormatProperties.optimalTilingFeatures){
					_context.logger().trace("Format \"{}\" suppord failed : required optimal tiling features not supported", formatStr);
					continue;
				}
			}

			finalFormat = format;
			break;
		}

		if (finalFormat == VK_FORMAT_UNDEFINED){
			_context.logger().warn("Failed to load format \"{}\" at line {}", name, mark.line);
			throw std::runtime_error("Failed to load format");
		}

		_context.registry().registerFormat(finalFormat, name);
	}

	void Loader::getCandidates(const YAML::Node& node, std::list<VkFormat>& candidates){
		YAML::Mark mark = node.Mark();

		for (const auto& formatNode : node){
			VkFormat format;
			try{
				format = formatNode.as<VkFormat>();
			} catch (const std::exception &e){
				_context.logger().warn("Failed to convert \"{}\" into a vulkan format at line {}", formatNode.as<std::string>(), mark.line);
				continue;
			}

			candidates.push_back(format);
		}
	}

	void Loader::getFeatures(const YAML::Node& node, VkFormatProperties& features){
		YAML::decodeVkFormatFeatureFlags(node["optimal"], features.optimalTilingFeatures);
		YAML::decodeVkFormatFeatureFlags(node["linear"], features.linearTilingFeatures);
		YAML::decodeVkFormatFeatureFlags(node["buffer"], features.bufferFeatures);
	}

	void Loader::getImageRequirements(const YAML::Node& node, ImageRequirements& requirements){
		requirements.type = node["type"].as<VkImageType>(VK_IMAGE_TYPE_2D);
		requirements.tiling = node["tiling"].as<VkImageTiling>(VK_IMAGE_TILING_OPTIMAL);
		YAML::decodeVkImageUsageFlags(node["usage"], requirements.usage);
		YAML::decodeVkImageCreateFlags(node["flags"], requirements.flags);

		auto& properties = requirements.properties;
		YAML::decodeVkSampleCountFlags(node["samples"], properties.sampleCounts);
		
		auto& extent = properties.maxExtent;
		extent.width = node["width"].as<uint32_t>(0);
		extent.height = node["height"].as<uint32_t>(0);
		extent.depth = node["depth"].as<uint32_t>(0);

		properties.maxArrayLayers = node["arrayLayers"].as<uint32_t>();
		properties.maxMipLevels = node["mipLevels"].as<uint32_t>();
	}

}