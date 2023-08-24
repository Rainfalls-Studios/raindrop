#ifndef __RAINDROP_GRAPHICS_CONFIGURATION_SCENE_HPP__
#define __RAINDROP_GRAPHICS_CONFIGURATION_SCENE_HPP__

#include "common.hpp"

namespace Raindrop::Graphics{
	static std::vector<AttachmentInfo> attachments = {
		// DEPTH
		{
			.description = VkAttachmentDescription{	
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D24_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM_S8_UINT,
			},

			.requiredFeatures = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.depthStencil = VkClearDepthStencilValue{
					.depth = 1.f,
					.stencil = 0,
				}
			},
		},

		// ALBEDO [RGB] + SPECULAR [A]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_UINT,
				VK_FORMAT_B8G8R8A8_UNORM,
				VK_FORMAT_A8B8G8R8_UINT_PACK32,
				VK_FORMAT_R8G8B8A8_SRGB,
			},

			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// BLOOM [RGB]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
				.queueFamilyIndexCount = 0,					// again.
				.pQueueFamilyIndices = nullptr,				// again
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,
			
			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// NORMAL [XYZ]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R16G16B16A16_SNORM,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},

		// POSITION [XYZ]
		{
			.description = VkAttachmentDescription{
				.flags = 0,
				.format = VK_FORMAT_UNDEFINED,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			},

			.imageView = VkImageViewCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.image = VK_NULL_HANDLE, // Set afterward
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.components = VkComponentMapping{},
				.subresourceRange = VkImageSubresourceRange{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},

			.image = VkImageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.imageType = VK_IMAGE_TYPE_2D,
				.format = VK_FORMAT_UNDEFINED,
				.extent = VkExtent3D{0, 0, 1}, // Set afterward
				.mipLevels = 1,
				.arrayLayers = 1,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.tiling = VK_IMAGE_TILING_OPTIMAL,
				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = 0,
				.pQueueFamilyIndices = nullptr,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			},

			.formats = {
				VK_FORMAT_R16G16B16_SFLOAT,
				VK_FORMAT_R16G16B16_UNORM,
				VK_FORMAT_R32G32B32_SFLOAT,
				VK_FORMAT_R32G32B32A32_SFLOAT,
				VK_FORMAT_R8G8B8A8_UNORM,
				VK_FORMAT_R8G8B8A8_SRGB,
			},
			
			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT,

			.clear = VkClearValue{
				.color = VkClearColorValue{
					.uint32 = {0U, 0U, 0U, 0U},
				}
			},
		},
	};
}

#endif