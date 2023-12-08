// #include "Scene.hpp"

// namespace Raindrop::Graphics{
// 	std::vector<AttachmentInfo> attachments = {
// 		// DEPTH
// 		{
// 			.description = VkAttachmentDescription{	
// 				.flags = 0,
// 				.format = VK_FORMAT_UNDEFINED,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 				.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
// 				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 			},

// 			.imageView = VkImageViewCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.image = VK_NULL_HANDLE, // Set afterward
// 				.viewType = VK_IMAGE_VIEW_TYPE_2D,
// 				.format = VK_FORMAT_UNDEFINED,
// 				.components = VkComponentMapping{},
// 				.subresourceRange = VkImageSubresourceRange{
// 					.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
// 					.baseMipLevel = 0,
// 					.levelCount = 1,
// 					.baseArrayLayer = 0,
// 					.layerCount = 1,
// 				},
// 			},

// 			.image = VkImageCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.imageType = VK_IMAGE_TYPE_2D,
// 				.format = VK_FORMAT_UNDEFINED,
// 				.extent = VkExtent3D{0, 0, 1}, // Set afterward
// 				.mipLevels = 1,
// 				.arrayLayers = 1,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.tiling = VK_IMAGE_TILING_OPTIMAL,
// 				.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
// 				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
// 				.queueFamilyIndexCount = 0,					// again.
// 				.pQueueFamilyIndices = nullptr,				// again
// 				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
// 			},

// 			.requiredFeatures = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
// 			.flags = DEPTH,

// 			.clear = VkClearValue{
// 				.depthStencil = VkClearDepthStencilValue{
// 					.depth = 1.f,
// 					.stencil = 0,
// 				}
// 			},

// 		},

// 		// ALBEDO [RGB] + SPECULAR [A]
// 		{
// 			.description = VkAttachmentDescription{
// 				.flags = 0,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
// 				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 			},

// 			.imageView = VkImageViewCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.image = VK_NULL_HANDLE, // Set afterward
// 				.viewType = VK_IMAGE_VIEW_TYPE_2D,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.components = VkComponentMapping{},
// 				.subresourceRange = VkImageSubresourceRange{
// 					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
// 					.baseMipLevel = 0,
// 					.levelCount = 1,
// 					.baseArrayLayer = 0,
// 					.layerCount = 1,
// 				},
// 			},

// 			.image = VkImageCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.imageType = VK_IMAGE_TYPE_2D,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.extent = VkExtent3D{0, 0, 1}, // Set afterward
// 				.mipLevels = 1,
// 				.arrayLayers = 1,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.tiling = VK_IMAGE_TILING_OPTIMAL,
// 				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
// 				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
// 				.queueFamilyIndexCount = 0,					// again.
// 				.pQueueFamilyIndices = nullptr,				// again
// 				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
// 			},

// 			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
// 			.flags = COLOR | RGBA,

// 			.clear = VkClearValue{
// 				.color = VkClearColorValue{
// 					.uint32 = {0U, 0U, 0U, 0U},
// 				}
// 			},
// 		},

// 		// BLOOM [RGB]
// 		{
// 			.description = VkAttachmentDescription{
// 				.flags = 0,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
// 				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 			},

// 			.imageView = VkImageViewCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.image = VK_NULL_HANDLE, // Set afterward
// 				.viewType = VK_IMAGE_VIEW_TYPE_2D,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.components = VkComponentMapping{},
// 				.subresourceRange = VkImageSubresourceRange{
// 					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
// 					.baseMipLevel = 0,
// 					.levelCount = 1,
// 					.baseArrayLayer = 0,
// 					.layerCount = 1,
// 				},
// 			},

// 			.image = VkImageCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.imageType = VK_IMAGE_TYPE_2D,
// 				.format = VK_FORMAT_R8G8B8A8_UNORM,
// 				.extent = VkExtent3D{0, 0, 1}, // Set afterward
// 				.mipLevels = 1,
// 				.arrayLayers = 1,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.tiling = VK_IMAGE_TILING_OPTIMAL,
// 				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
// 				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,	// Same, set afterward
// 				.queueFamilyIndexCount = 0,					// again.
// 				.pQueueFamilyIndices = nullptr,				// again
// 				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
// 			},

// 			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
// 			.flags = COLOR | RGB | RGBA,
			
// 			.clear = VkClearValue{
// 				.color = VkClearColorValue{
// 					.uint32 = {0U, 0U, 0U, 0U},
// 				}
// 			},
// 		},

// 		// NORMAL [XYZ]
// 		{
// 			.description = VkAttachmentDescription{
// 				.flags = 0,
// 				.format = VK_FORMAT_R16G16B16_SNORM,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
// 				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 			},

// 			.imageView = VkImageViewCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.image = VK_NULL_HANDLE, // Set afterward
// 				.viewType = VK_IMAGE_VIEW_TYPE_2D,
// 				.format = VK_FORMAT_R16G16B16_SNORM,
// 				.components = VkComponentMapping{},
// 				.subresourceRange = VkImageSubresourceRange{
// 					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
// 					.baseMipLevel = 0,
// 					.levelCount = 1,
// 					.baseArrayLayer = 0,
// 					.layerCount = 1,
// 				},
// 			},

// 			.image = VkImageCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.imageType = VK_IMAGE_TYPE_2D,
// 				.format = VK_FORMAT_R16G16B16_SNORM,
// 				.extent = VkExtent3D{0, 0, 1}, // Set afterward
// 				.mipLevels = 1,
// 				.arrayLayers = 1,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.tiling = VK_IMAGE_TILING_OPTIMAL,
// 				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
// 				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
// 				.queueFamilyIndexCount = 0,
// 				.pQueueFamilyIndices = nullptr,
// 				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
// 			},
			
// 			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
// 			.flags = FLOAT | RGB | RGBA,

// 			.clear = VkClearValue{
// 				.color = VkClearColorValue{
// 					.uint32 = {0U, 0U, 0U, 0U},
// 				}
// 			},
// 		},

// 		// POSITION [XYZ]
// 		{
// 			.description = VkAttachmentDescription{
// 				.flags = 0,
// 				.format = VK_FORMAT_R32G32B32_SFLOAT,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
// 				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
// 				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
// 				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
// 				.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
// 				.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
// 			},

// 			.imageView = VkImageViewCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.image = VK_NULL_HANDLE, // Set afterward
// 				.viewType = VK_IMAGE_VIEW_TYPE_2D,
// 				.format = VK_FORMAT_R32G32B32_SFLOAT,
// 				.components = VkComponentMapping{},
// 				.subresourceRange = VkImageSubresourceRange{
// 					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
// 					.baseMipLevel = 0,
// 					.levelCount = 1,
// 					.baseArrayLayer = 0,
// 					.layerCount = 1,
// 				},
// 			},

// 			.image = VkImageCreateInfo{
// 				.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
// 				.pNext = nullptr,
// 				.flags = 0,
// 				.imageType = VK_IMAGE_TYPE_2D,
// 				.format = VK_FORMAT_R32G32B32_SFLOAT,
// 				.extent = VkExtent3D{0, 0, 1}, // Set afterward
// 				.mipLevels = 1,
// 				.arrayLayers = 1,
// 				.samples = VK_SAMPLE_COUNT_1_BIT,
// 				.tiling = VK_IMAGE_TILING_OPTIMAL,
// 				.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
// 				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
// 				.queueFamilyIndexCount = 0,
// 				.pQueueFamilyIndices = nullptr,
// 				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
// 			},

// 			.requiredFeatures = VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT,
// 			.flags = FLOAT | RGB | RGBA,
			
// 			.clear = VkClearValue{
// 				.color = VkClearColorValue{
// 					.uint32 = {0U, 0U, 0U, 0U},
// 				}
// 			},
// 		},
// 	};
// }