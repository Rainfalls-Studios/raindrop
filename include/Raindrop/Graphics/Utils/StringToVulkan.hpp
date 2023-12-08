#ifndef __RAINDROP_GRAPHICS_UTILS_STRING_TO_VULKAN_HPP__
#define __RAINDROP_GRAPHICS_UTILS_STRING_TO_VULKAN_HPP__

#include <Raindrop/Graphics/Utils/common.hpp>

#define MTH_STR_TO_TYPE(type) static type strTo##type(const std::string& str);
#define MTH_STR_FROM_TYPE(type) static const char* type##ToStr(const type& value);
#define MTH_TYPE_PAIR(type) MTH_STR_TO_TYPE(type); MTH_STR_FROM_TYPE(type);

namespace Raindrop::Graphics::Utils{
	class StringToVulkan{
		public:
			MTH_TYPE_PAIR(VkResult);
			MTH_TYPE_PAIR(VkPipelineCacheHeaderVersion);
			MTH_TYPE_PAIR(VkImageLayout);
			MTH_TYPE_PAIR(VkVendorId);
			MTH_TYPE_PAIR(VkSystemAllocationScope);
			MTH_TYPE_PAIR(VkFormat);
			MTH_TYPE_PAIR(VkImageTiling);
			MTH_TYPE_PAIR(VkImageType);
			MTH_TYPE_PAIR(VkPhysicalDeviceType);
			MTH_TYPE_PAIR(VkQueryType);
			MTH_TYPE_PAIR(VkSharingMode);
			MTH_TYPE_PAIR(VkComponentSwizzle);
			MTH_TYPE_PAIR(VkImageViewType);
			MTH_TYPE_PAIR(VkBlendFactor);
			MTH_TYPE_PAIR(VkBlendOp);
			MTH_TYPE_PAIR(VkCompareOp);
			MTH_TYPE_PAIR(VkDynamicState);
			MTH_TYPE_PAIR(VkFrontFace);
			MTH_TYPE_PAIR(VkVertexInputRate);
			MTH_TYPE_PAIR(VkPrimitiveTopology);
			MTH_TYPE_PAIR(VkPolygonMode);
			MTH_TYPE_PAIR(VkStencilOp);
			MTH_TYPE_PAIR(VkLogicOp);
			MTH_TYPE_PAIR(VkBorderColor);
			MTH_TYPE_PAIR(VkFilter);
			MTH_TYPE_PAIR(VkSamplerAddressMode);
			MTH_TYPE_PAIR(VkSamplerMipmapMode);
			MTH_TYPE_PAIR(VkDescriptorType);
			MTH_TYPE_PAIR(VkAttachmentLoadOp);
			MTH_TYPE_PAIR(VkAttachmentStoreOp);
			MTH_TYPE_PAIR(VkPipelineBindPoint);
			MTH_TYPE_PAIR(VkCommandBufferLevel);
			MTH_TYPE_PAIR(VkIndexType);
			MTH_TYPE_PAIR(VkSubpassContents);
			MTH_TYPE_PAIR(VkAccessFlagBits);
			MTH_TYPE_PAIR(VkImageAspectFlagBits);
			MTH_TYPE_PAIR(VkFormatFeatureFlagBits);
			MTH_TYPE_PAIR(VkImageCreateFlagBits);
			MTH_TYPE_PAIR(VkSampleCountFlagBits);
			MTH_TYPE_PAIR(VkImageUsageFlagBits);
			MTH_TYPE_PAIR(VkMemoryHeapFlagBits);
			MTH_TYPE_PAIR(VkMemoryPropertyFlagBits);
			MTH_TYPE_PAIR(VkQueueFlagBits);
			MTH_TYPE_PAIR(VkPipelineStageFlagBits);
			MTH_TYPE_PAIR(VkSparseMemoryBindFlagBits);
			MTH_TYPE_PAIR(VkSparseImageFormatFlagBits);
			MTH_TYPE_PAIR(VkFenceCreateFlagBits);
			MTH_TYPE_PAIR(VkEventCreateFlagBits);
			MTH_TYPE_PAIR(VkQueryPipelineStatisticFlagBits);
			MTH_TYPE_PAIR(VkQueryResultFlagBits);
			MTH_TYPE_PAIR(VkBufferCreateFlagBits);
			MTH_TYPE_PAIR(VkBufferUsageFlagBits);
			MTH_TYPE_PAIR(VkImageViewCreateFlagBits);
			MTH_TYPE_PAIR(VkPipelineCacheCreateFlagBits);
			MTH_TYPE_PAIR(VkColorComponentFlagBits);
			MTH_TYPE_PAIR(VkPipelineCreateFlagBits);
			MTH_TYPE_PAIR(VkPipelineShaderStageCreateFlagBits);
			MTH_TYPE_PAIR(VkShaderStageFlagBits);
			MTH_TYPE_PAIR(VkCullModeFlagBits);
			MTH_TYPE_PAIR(VkPipelineDepthStencilStateCreateFlagBits);
			MTH_TYPE_PAIR(VkPipelineColorBlendStateCreateFlagBits);
			MTH_TYPE_PAIR(VkPipelineLayoutCreateFlagBits);
			MTH_TYPE_PAIR(VkSamplerCreateFlagBits);
			MTH_TYPE_PAIR(VkDescriptorPoolCreateFlagBits);
			MTH_TYPE_PAIR(VkDescriptorSetLayoutCreateFlagBits);
			MTH_TYPE_PAIR(VkAttachmentDescriptionFlagBits);
			MTH_TYPE_PAIR(VkDependencyFlagBits);
			MTH_TYPE_PAIR(VkFramebufferCreateFlagBits);
			MTH_TYPE_PAIR(VkRenderPassCreateFlagBits);
			MTH_TYPE_PAIR(VkSubpassDescriptionFlagBits);
			MTH_TYPE_PAIR(VkCommandPoolCreateFlagBits);
			MTH_TYPE_PAIR(VkCommandPoolResetFlagBits);
			MTH_TYPE_PAIR(VkCommandBufferUsageFlagBits);
			MTH_TYPE_PAIR(VkQueryControlFlagBits);
			MTH_TYPE_PAIR(VkCommandBufferResetFlagBits);
			MTH_TYPE_PAIR(VkStencilFaceFlagBits);

	};
}

#undef MTH_STR_TO_TYPE
#undef MTH_STR_FROM_TYPE
#undef MTH_TYPE_PAIR

#define YAML_CONVERTER_UNIQUE(type) \
	namespace YAML{ \
		template<> \
		struct convert<type>{ \
			static Node encode(const type& value){ \
				Node node; \
				node.push_back(Raindrop::Graphics::Utils::StringToVulkan::type##ToStr(value)); \
				return node; \
			} \
			static bool decode(const Node& node, type& value){ \
				value = Raindrop::Graphics::Utils::StringToVulkan::strTo##type(node.as<std::string>()); \
				return true; \
			} \
		}; \
	}

#define YAML_CONVERTER_COMBINED(type, bitType) \
	namespace YAML{ \
		static Node encode##type(const type& value){ \
			Node node; \
			for (std::size_t i=0; i<sizeof(type)/8; i++){ \
				type bit = type(1 << i); \
				node.push_back(Raindrop::Graphics::Utils::StringToVulkan::bitType##ToStr(static_cast<bitType>(bit))); \
			} \
			return node; \
		} \
		static bool decode##type(const Node& node, type& value){ \
			value = type(0); \
			for (const auto& child : node){ \
				value |= static_cast<type>(Raindrop::Graphics::Utils::StringToVulkan::strTo##bitType(child.as<std::string>())); \
			} \
			return true; \
		} \
	}

YAML_CONVERTER_UNIQUE(VkResult);
YAML_CONVERTER_UNIQUE(VkPipelineCacheHeaderVersion);
YAML_CONVERTER_UNIQUE(VkImageLayout);
YAML_CONVERTER_UNIQUE(VkVendorId);
YAML_CONVERTER_UNIQUE(VkSystemAllocationScope);
YAML_CONVERTER_UNIQUE(VkFormat);
YAML_CONVERTER_UNIQUE(VkImageTiling);
YAML_CONVERTER_UNIQUE(VkImageType);
YAML_CONVERTER_UNIQUE(VkPhysicalDeviceType);
YAML_CONVERTER_UNIQUE(VkQueryType);
YAML_CONVERTER_UNIQUE(VkSharingMode);
YAML_CONVERTER_UNIQUE(VkComponentSwizzle);
YAML_CONVERTER_UNIQUE(VkImageViewType);
YAML_CONVERTER_UNIQUE(VkBlendFactor);
YAML_CONVERTER_UNIQUE(VkBlendOp);
YAML_CONVERTER_UNIQUE(VkCompareOp);
YAML_CONVERTER_UNIQUE(VkDynamicState);
YAML_CONVERTER_UNIQUE(VkFrontFace);
YAML_CONVERTER_UNIQUE(VkVertexInputRate);
YAML_CONVERTER_UNIQUE(VkPrimitiveTopology);
YAML_CONVERTER_UNIQUE(VkPolygonMode);
YAML_CONVERTER_UNIQUE(VkStencilOp);
YAML_CONVERTER_UNIQUE(VkLogicOp);
YAML_CONVERTER_UNIQUE(VkBorderColor);
YAML_CONVERTER_UNIQUE(VkFilter);
YAML_CONVERTER_UNIQUE(VkSamplerAddressMode);
YAML_CONVERTER_UNIQUE(VkSamplerMipmapMode);
YAML_CONVERTER_UNIQUE(VkDescriptorType);
YAML_CONVERTER_UNIQUE(VkAttachmentLoadOp);
YAML_CONVERTER_UNIQUE(VkAttachmentStoreOp);
YAML_CONVERTER_UNIQUE(VkPipelineBindPoint);
YAML_CONVERTER_UNIQUE(VkCommandBufferLevel);
YAML_CONVERTER_UNIQUE(VkIndexType);
YAML_CONVERTER_UNIQUE(VkSubpassContents);
YAML_CONVERTER_UNIQUE(VkAccessFlagBits);
YAML_CONVERTER_UNIQUE(VkImageAspectFlagBits);
YAML_CONVERTER_UNIQUE(VkFormatFeatureFlagBits);
YAML_CONVERTER_UNIQUE(VkImageCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkSampleCountFlagBits);
YAML_CONVERTER_UNIQUE(VkImageUsageFlagBits);
YAML_CONVERTER_UNIQUE(VkMemoryHeapFlagBits);
YAML_CONVERTER_UNIQUE(VkMemoryPropertyFlagBits);
YAML_CONVERTER_UNIQUE(VkQueueFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineStageFlagBits);
YAML_CONVERTER_UNIQUE(VkSparseMemoryBindFlagBits);
YAML_CONVERTER_UNIQUE(VkSparseImageFormatFlagBits);
YAML_CONVERTER_UNIQUE(VkFenceCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkEventCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkQueryPipelineStatisticFlagBits);
YAML_CONVERTER_UNIQUE(VkQueryResultFlagBits);
YAML_CONVERTER_UNIQUE(VkBufferCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkBufferUsageFlagBits);
YAML_CONVERTER_UNIQUE(VkImageViewCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineCacheCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkColorComponentFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineShaderStageCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkShaderStageFlagBits);
YAML_CONVERTER_UNIQUE(VkCullModeFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineDepthStencilStateCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineColorBlendStateCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkPipelineLayoutCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkSamplerCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkDescriptorPoolCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkDescriptorSetLayoutCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkAttachmentDescriptionFlagBits);
YAML_CONVERTER_UNIQUE(VkDependencyFlagBits);
YAML_CONVERTER_UNIQUE(VkFramebufferCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkRenderPassCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkSubpassDescriptionFlagBits);
YAML_CONVERTER_UNIQUE(VkCommandPoolCreateFlagBits);
YAML_CONVERTER_UNIQUE(VkCommandPoolResetFlagBits);
YAML_CONVERTER_UNIQUE(VkCommandBufferUsageFlagBits);
YAML_CONVERTER_UNIQUE(VkQueryControlFlagBits);
YAML_CONVERTER_UNIQUE(VkCommandBufferResetFlagBits);
YAML_CONVERTER_UNIQUE(VkStencilFaceFlagBits);

YAML_CONVERTER_COMBINED(VkAccessFlags, VkAccessFlagBits);
YAML_CONVERTER_COMBINED(VkImageAspectFlags, VkImageAspectFlagBits);
YAML_CONVERTER_COMBINED(VkFormatFeatureFlags, VkFormatFeatureFlagBits);
YAML_CONVERTER_COMBINED(VkImageCreateFlags, VkImageCreateFlagBits);
YAML_CONVERTER_COMBINED(VkSampleCountFlags, VkSampleCountFlagBits);
YAML_CONVERTER_COMBINED(VkImageUsageFlags, VkImageUsageFlagBits);
YAML_CONVERTER_COMBINED(VkMemoryHeapFlags, VkMemoryHeapFlagBits);
YAML_CONVERTER_COMBINED(VkMemoryPropertyFlags, VkMemoryPropertyFlagBits);
YAML_CONVERTER_COMBINED(VkQueueFlags, VkQueueFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineStageFlags, VkPipelineStageFlagBits);
YAML_CONVERTER_COMBINED(VkSparseMemoryBindFlags, VkSparseMemoryBindFlagBits);
YAML_CONVERTER_COMBINED(VkSparseImageFormatFlags, VkSparseImageFormatFlagBits);
YAML_CONVERTER_COMBINED(VkFenceCreateFlags, VkFenceCreateFlagBits);
YAML_CONVERTER_COMBINED(VkEventCreateFlags, VkEventCreateFlagBits);
YAML_CONVERTER_COMBINED(VkQueryPipelineStatisticFlags, VkQueryPipelineStatisticFlagBits);
YAML_CONVERTER_COMBINED(VkQueryResultFlags, VkQueryResultFlagBits);
YAML_CONVERTER_COMBINED(VkBufferCreateFlags, VkBufferCreateFlagBits);
YAML_CONVERTER_COMBINED(VkBufferUsageFlags, VkBufferUsageFlagBits);
YAML_CONVERTER_COMBINED(VkImageViewCreateFlags, VkImageViewCreateFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineCacheCreateFlags, VkPipelineCacheCreateFlagBits);
YAML_CONVERTER_COMBINED(VkColorComponentFlags, VkColorComponentFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineCreateFlags, VkPipelineCreateFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineShaderStageCreateFlags, VkPipelineShaderStageCreateFlagBits);
YAML_CONVERTER_COMBINED(VkShaderStageFlags, VkShaderStageFlagBits);
YAML_CONVERTER_COMBINED(VkCullModeFlags, VkCullModeFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineDepthStencilStateCreateFlags, VkPipelineDepthStencilStateCreateFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineColorBlendStateCreateFlags, VkPipelineColorBlendStateCreateFlagBits);
YAML_CONVERTER_COMBINED(VkPipelineLayoutCreateFlags, VkPipelineLayoutCreateFlagBits);
YAML_CONVERTER_COMBINED(VkSamplerCreateFlags, VkSamplerCreateFlagBits);
YAML_CONVERTER_COMBINED(VkDescriptorPoolCreateFlags, VkDescriptorPoolCreateFlagBits);
YAML_CONVERTER_COMBINED(VkDescriptorSetLayoutCreateFlags, VkDescriptorSetLayoutCreateFlagBits);
YAML_CONVERTER_COMBINED(VkAttachmentDescriptionFlags, VkAttachmentDescriptionFlagBits);
YAML_CONVERTER_COMBINED(VkDependencyFlags, VkDependencyFlagBits);
YAML_CONVERTER_COMBINED(VkFramebufferCreateFlags, VkFramebufferCreateFlagBits);
YAML_CONVERTER_COMBINED(VkRenderPassCreateFlags, VkRenderPassCreateFlagBits);
YAML_CONVERTER_COMBINED(VkSubpassDescriptionFlags, VkSubpassDescriptionFlagBits);
YAML_CONVERTER_COMBINED(VkCommandPoolCreateFlags, VkCommandPoolCreateFlagBits);
YAML_CONVERTER_COMBINED(VkCommandPoolResetFlags, VkCommandPoolResetFlagBits);
YAML_CONVERTER_COMBINED(VkCommandBufferUsageFlags, VkCommandBufferUsageFlagBits);
YAML_CONVERTER_COMBINED(VkQueryControlFlags, VkQueryControlFlagBits);
YAML_CONVERTER_COMBINED(VkCommandBufferResetFlags, VkCommandBufferResetFlagBits);
YAML_CONVERTER_COMBINED(VkStencilFaceFlags, VkStencilFaceFlagBits);

#undef YAML_CONVERTER_UNIQUE
#undef YAML_CONVERTER_COMBINED

#endif