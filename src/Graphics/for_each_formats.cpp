// generated using scrips/generate_formats_loops.py
#define for_each_formats(fnc) for (VkFormat fmt = VK_FORMAT_R4G4_UNORM_PACK8; fmt <= VK_FORMAT_ASTC_12x12_SRGB_BLOCK; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_G8B8G8R8_422_UNORM; fmt <= VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_G8_B8R8_2PLANE_444_UNORM; fmt <= VK_FORMAT_G16_B16R16_2PLANE_444_UNORM; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_A4R4G4B4_UNORM_PACK16; fmt <= VK_FORMAT_A4B4G4R4_UNORM_PACK16; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK; fmt <= VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG; fmt <= VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_R16G16_S10_5_NV; fmt <= VK_FORMAT_R16G16_S10_5_NV; fmt = static_cast<VkFormat>(fmt + 1)){fnc}for (VkFormat fmt = VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR; fmt <= VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT; fmt = static_cast<VkFormat>(fmt + 1)){fnc}