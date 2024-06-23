#ifndef __RAINDROP_INTERNAL_TEXTURE_HPP__
#define __RAINDROP_INTERNAL_TEXTURE_HPP__

#include <Raindrop/Texture.hpp>
#include <Raindrop_internal/Graphics/Image.hpp>

namespace Raindrop{
	struct Texture::Impl{

		Internal::Context* context;

		// Read only
		Format format;
		std::uint32_t width;
		std::uint32_t height;
		std::uint32_t depth;
		Usage usage;
		Layout initialLayout;
		Tiling tiling;
		Type type;
		std::uint32_t mipmapCount;
		std::uint32_t layerCount;
		Flags flags;

		// dynamic
		Layout currentLayout;

		std::shared_ptr<Internal::Graphics::Image> internal;

		Impl() : 
			context{nullptr},
			format{Format::UNDEFINED},
			width{1},
			height{1},
			depth{1},
			usage{Usage::NONE},
			initialLayout{Layout::UNDEFINED},
			tiling{Tiling::OPTIMAL},
			mipmapCount{1},
			layerCount{1},
			currentLayout{Layout::UNDEFINED},
			internal{nullptr}
		{}
	};

	constexpr VkImageAspectFlags TextureAspectToVulkan(const Texture::Aspect& a){
		VkImageAspectFlags out = VK_IMAGE_ASPECT_NONE;

		if (a.has(Texture::Aspect::COLOR)) out |= VK_IMAGE_ASPECT_COLOR_BIT;
		if (a.has(Texture::Aspect::DEPTH)) out |= VK_IMAGE_ASPECT_DEPTH_BIT;
		if (a.has(Texture::Aspect::STENCIL)) out |= VK_IMAGE_ASPECT_STENCIL_BIT;
		if (a.has(Texture::Aspect::METADATA)) out |= VK_IMAGE_ASPECT_METADATA_BIT;
		if (a.has(Texture::Aspect::PLANE_0)) out |= VK_IMAGE_ASPECT_PLANE_0_BIT;
		if (a.has(Texture::Aspect::PLANE_1)) out |= VK_IMAGE_ASPECT_PLANE_1_BIT;
		if (a.has(Texture::Aspect::PLANE_2)) out |= VK_IMAGE_ASPECT_PLANE_2_BIT;

		// VK_EXT_image_drm_format_modifier
		if (a.has(Texture::Aspect::MEMORY_PLANE_0)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_1)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_2)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
		if (a.has(Texture::Aspect::MEMORY_PLANE_3)) out |= VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;

		return out;
	}

	constexpr Texture::Aspect TextureAspectToRaindrop(const VkImageAspectFlags& flags){
		Texture::Aspect out = Texture::Aspect::NONE;

		if (flags & VK_IMAGE_ASPECT_COLOR_BIT) out.set(Texture::Aspect::COLOR);
		if (flags & VK_IMAGE_ASPECT_DEPTH_BIT) out.set(Texture::Aspect::DEPTH);
		if (flags & VK_IMAGE_ASPECT_STENCIL_BIT) out.set(Texture::Aspect::STENCIL);
		if (flags & VK_IMAGE_ASPECT_METADATA_BIT) out.set(Texture::Aspect::METADATA);
		if (flags & VK_IMAGE_ASPECT_PLANE_0_BIT) out.set(Texture::Aspect::PLANE_0);
		if (flags & VK_IMAGE_ASPECT_PLANE_1_BIT) out.set(Texture::Aspect::PLANE_1);
		if (flags & VK_IMAGE_ASPECT_PLANE_2_BIT) out.set(Texture::Aspect::PLANE_2);
		if (flags & VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT) out.set(Texture::Aspect::MEMORY_PLANE_0);
		if (flags & VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT) out.set(Texture::Aspect::MEMORY_PLANE_1);
		if (flags & VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT) out.set(Texture::Aspect::MEMORY_PLANE_2);
		if (flags & VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT) out.set(Texture::Aspect::MEMORY_PLANE_3);

		return out;
	}

	constexpr VkImageAspectFlagBits TextureAspectBitToVulkan(const Texture::Aspect::Bits& b){
		switch (b){
			case Texture::Aspect::COLOR: return VK_IMAGE_ASPECT_COLOR_BIT;
			case Texture::Aspect::DEPTH: return VK_IMAGE_ASPECT_DEPTH_BIT;
			case Texture::Aspect::STENCIL: return VK_IMAGE_ASPECT_STENCIL_BIT;
			case Texture::Aspect::METADATA: return VK_IMAGE_ASPECT_METADATA_BIT;
			case Texture::Aspect::PLANE_0: return VK_IMAGE_ASPECT_PLANE_0_BIT;
			case Texture::Aspect::PLANE_1: return VK_IMAGE_ASPECT_PLANE_1_BIT;
			case Texture::Aspect::PLANE_2: return VK_IMAGE_ASPECT_PLANE_2_BIT;
			case Texture::Aspect::MEMORY_PLANE_0: return VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT;
			case Texture::Aspect::MEMORY_PLANE_1: return VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT;
			case Texture::Aspect::MEMORY_PLANE_2: return VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT;
			case Texture::Aspect::MEMORY_PLANE_3: return VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT;
			default: return VK_IMAGE_ASPECT_NONE;
		}
	}

	constexpr Texture::Aspect::Bits TextureAspectBitToRaindrop(const VkImageAspectFlagBits& b){
		switch (b){
			case VK_IMAGE_ASPECT_COLOR_BIT: return Texture::Aspect::COLOR;
			case VK_IMAGE_ASPECT_DEPTH_BIT: return Texture::Aspect::DEPTH;
			case VK_IMAGE_ASPECT_STENCIL_BIT: return Texture::Aspect::STENCIL;
			case VK_IMAGE_ASPECT_METADATA_BIT: return Texture::Aspect::METADATA;
			case VK_IMAGE_ASPECT_PLANE_0_BIT: return Texture::Aspect::PLANE_0;
			case VK_IMAGE_ASPECT_PLANE_1_BIT: return Texture::Aspect::PLANE_1;
			case VK_IMAGE_ASPECT_PLANE_2_BIT: return Texture::Aspect::PLANE_2;
			case VK_IMAGE_ASPECT_MEMORY_PLANE_0_BIT_EXT: return Texture::Aspect::MEMORY_PLANE_0;
			case VK_IMAGE_ASPECT_MEMORY_PLANE_1_BIT_EXT: return Texture::Aspect::MEMORY_PLANE_1;
			case VK_IMAGE_ASPECT_MEMORY_PLANE_2_BIT_EXT: return Texture::Aspect::MEMORY_PLANE_2;
			case VK_IMAGE_ASPECT_MEMORY_PLANE_3_BIT_EXT: return Texture::Aspect::MEMORY_PLANE_3;
			default: return Texture::Aspect::NONE;
		}
	}

	constexpr VkImageCreateFlags TextureFlagsToVulkan(const Texture::Flags& flags){
		VkImageCreateFlags out = 0;

		if (flags.has(Texture::Flags::SPARSE_BINDING)) out |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
		if (flags.has(Texture::Flags::SPARSE_RESIDENCY)) out |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
		if (flags.has(Texture::Flags::SPARSE_ALIASED)) out |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
		if (flags.has(Texture::Flags::MUTABLE_FORMAT)) out |= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		if (flags.has(Texture::Flags::CUBE_COMPATIBLE)) out |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::PLANAR_ARRAY_COMPATIBLE)) out |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::ALIAS)) out |= VK_IMAGE_CREATE_ALIAS_BIT;
		if (flags.has(Texture::Flags::SPLIT_INSTANCE_BIND_REGIONS)) out |= VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
		if (flags.has(Texture::Flags::BLOCK_TEXEL_VIEW_COMPATIBLE)) out |= VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
		if (flags.has(Texture::Flags::EXTENDED_USAGE)) out |= VK_IMAGE_CREATE_EXTENDED_USAGE_BIT;
		if (flags.has(Texture::Flags::PROTECTED)) out |= VK_IMAGE_CREATE_PROTECTED_BIT;
		if (flags.has(Texture::Flags::DISJOINT)) out |= VK_IMAGE_CREATE_DISJOINT_BIT;

		// VK_NV_corner_sampled_image
		if (flags.has(Texture::Flags::CORNER_SAMPLED)) out |= VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV;

		// VK_EXT_sample_locations
		if (flags.has(Texture::Flags::SAMPLE_LOCATIONS_COMPATIBLE_DEPTH)) out |= VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT;

		// VK_EXT_fragment_density_map
		if (flags.has(Texture::Flags::SUBSAMPLED)) out |= VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT;

		// VK_EXT_descriptor_buffer
		if (flags.has(Texture::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY)) out |= VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;

		// VK_EXT_multisampled_render_to_single_sampled
		if (flags.has(Texture::Flags::MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED)) out |= VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT;

		// VK_EXT_image_2d_view_of_3d
		if (flags.has(Texture::Flags::PLANAR_VIEW_COMPATIBLE)) out |= VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;

		// VK_QCOM_fragment_density_map_offset
		if (flags.has(Texture::Flags::FRAGMENT_DENSITY_MAP_OFFSET)) out |= VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM;

		// VK_KHR_video_maintenance1
		if (flags.has(Texture::Flags::VIDEO_PROFILE_INDEPENDENT)) out |= VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR;

		return out;
	}

	constexpr Texture::Flags TextureFlagsToRaindrop(const VkImageCreateFlags& flags){
		Texture::Flags out = Texture::Flags::NONE;

		if (flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT) out.set(Texture::Flags::SPARSE_BINDING);
		if (flags & VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT) out.set(Texture::Flags::SPARSE_RESIDENCY);
		if (flags & VK_IMAGE_CREATE_SPARSE_ALIASED_BIT) out.set(Texture::Flags::SPARSE_ALIASED);
		if (flags & VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT) out.set(Texture::Flags::MUTABLE_FORMAT);
		if (flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) out.set(Texture::Flags::CUBE_COMPATIBLE);
		if (flags & VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT) out.set(Texture::Flags::PLANAR_ARRAY_COMPATIBLE);
		if (flags & VK_IMAGE_CREATE_ALIAS_BIT) out.set(Texture::Flags::ALIAS);
		if (flags & VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT) out.set(Texture::Flags::SPLIT_INSTANCE_BIND_REGIONS);
		if (flags & VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT) out.set(Texture::Flags::BLOCK_TEXEL_VIEW_COMPATIBLE);
		if (flags & VK_IMAGE_CREATE_EXTENDED_USAGE_BIT) out.set(Texture::Flags::EXTENDED_USAGE);
		if (flags & VK_IMAGE_CREATE_PROTECTED_BIT) out.set(Texture::Flags::PROTECTED);
		if (flags & VK_IMAGE_CREATE_DISJOINT_BIT) out.set(Texture::Flags::DISJOINT);
		if (flags & VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV) out.set(Texture::Flags::CORNER_SAMPLED);
		if (flags & VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT) out.set(Texture::Flags::SAMPLE_LOCATIONS_COMPATIBLE_DEPTH);
		if (flags & VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT) out.set(Texture::Flags::SUBSAMPLED);
		if (flags & VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT) out.set(Texture::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY);
		if (flags & VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT) out.set(Texture::Flags::MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED);
		if (flags & VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT) out.set(Texture::Flags::PLANAR_VIEW_COMPATIBLE);
		if (flags & VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM) out.set(Texture::Flags::FRAGMENT_DENSITY_MAP_OFFSET);
		if (flags & VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR) out.set(Texture::Flags::VIDEO_PROFILE_INDEPENDENT);

		return out;
	}

	constexpr VkImageCreateFlagBits TextureFlagsBitsToVulkan(const Texture::Flags::Bits& flags){
		switch (flags){
			case Texture::Flags::SPARSE_BINDING: return VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
			case Texture::Flags::SPARSE_RESIDENCY: return VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
			case Texture::Flags::SPARSE_ALIASED: return VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
			case Texture::Flags::MUTABLE_FORMAT: return VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
			case Texture::Flags::CUBE_COMPATIBLE: return VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			case Texture::Flags::PLANAR_ARRAY_COMPATIBLE: return VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
			case Texture::Flags::ALIAS: return VK_IMAGE_CREATE_ALIAS_BIT;
			case Texture::Flags::SPLIT_INSTANCE_BIND_REGIONS: return VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
			case Texture::Flags::BLOCK_TEXEL_VIEW_COMPATIBLE: return VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
			case Texture::Flags::EXTENDED_USAGE: return VK_IMAGE_CREATE_EXTENDED_USAGE_BIT;
			case Texture::Flags::PROTECTED: return VK_IMAGE_CREATE_PROTECTED_BIT;
			case Texture::Flags::DISJOINT: return VK_IMAGE_CREATE_DISJOINT_BIT;
			case Texture::Flags::CORNER_SAMPLED: return VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV;
			case Texture::Flags::SAMPLE_LOCATIONS_COMPATIBLE_DEPTH: return VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT;
			case Texture::Flags::SUBSAMPLED: return VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT;
			case Texture::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY: return VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;
			case Texture::Flags::MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED: return VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT;
			case Texture::Flags::PLANAR_VIEW_COMPATIBLE: return VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT;
			case Texture::Flags::FRAGMENT_DENSITY_MAP_OFFSET: return VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM;
			case Texture::Flags::VIDEO_PROFILE_INDEPENDENT: return VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR;
			default: return VkImageCreateFlagBits(0);
		}
	}

	constexpr Texture::Flags::Bits TextureFlagsBitsToRaindrop(const VkImageCreateFlagBits& flags){
		switch (flags){
			case VK_IMAGE_CREATE_SPARSE_BINDING_BIT: return Texture::Flags::SPARSE_BINDING;
			case VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT: return Texture::Flags::SPARSE_RESIDENCY;
			case VK_IMAGE_CREATE_SPARSE_ALIASED_BIT: return Texture::Flags::SPARSE_ALIASED;
			case VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT: return Texture::Flags::MUTABLE_FORMAT;
			case VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT: return Texture::Flags::CUBE_COMPATIBLE;
			case VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT: return Texture::Flags::PLANAR_ARRAY_COMPATIBLE;
			case VK_IMAGE_CREATE_ALIAS_BIT: return Texture::Flags::ALIAS;
			case VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT: return Texture::Flags::SPLIT_INSTANCE_BIND_REGIONS;
			case VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT: return Texture::Flags::BLOCK_TEXEL_VIEW_COMPATIBLE;
			case VK_IMAGE_CREATE_EXTENDED_USAGE_BIT: return Texture::Flags::EXTENDED_USAGE;
			case VK_IMAGE_CREATE_PROTECTED_BIT: return Texture::Flags::PROTECTED;
			case VK_IMAGE_CREATE_DISJOINT_BIT: return Texture::Flags::DISJOINT;
			case VK_IMAGE_CREATE_CORNER_SAMPLED_BIT_NV: return Texture::Flags::CORNER_SAMPLED;
			case VK_IMAGE_CREATE_SAMPLE_LOCATIONS_COMPATIBLE_DEPTH_BIT_EXT: return Texture::Flags::SAMPLE_LOCATIONS_COMPATIBLE_DEPTH;
			case VK_IMAGE_CREATE_SUBSAMPLED_BIT_EXT: return Texture::Flags::SUBSAMPLED;
			case VK_IMAGE_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT: return Texture::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY;
			case VK_IMAGE_CREATE_MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED_BIT_EXT: return Texture::Flags::MULTISAMPLED_RENDER_TO_SINGLE_SAMPLED;
			case VK_IMAGE_CREATE_2D_VIEW_COMPATIBLE_BIT_EXT: return Texture::Flags::PLANAR_VIEW_COMPATIBLE;
			case VK_IMAGE_CREATE_FRAGMENT_DENSITY_MAP_OFFSET_BIT_QCOM: return Texture::Flags::FRAGMENT_DENSITY_MAP_OFFSET;
			case VK_IMAGE_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR: return Texture::Flags::VIDEO_PROFILE_INDEPENDENT;
			default: return Texture::Flags::NONE;
		}
	}

	constexpr VkImageUsageFlags TextureUsageToVulkan(const Texture::Usage& u){
		VkImageUsageFlags out = 0;
		
		if (u.has(Texture::Usage::TRANSFER_SRC)) out |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (u.has(Texture::Usage::TRANSFER_DST)) out |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		if (u.has(Texture::Usage::SAMPLED)) out |= VK_IMAGE_USAGE_SAMPLED_BIT;
		if (u.has(Texture::Usage::STORAGE)) out |= VK_IMAGE_USAGE_STORAGE_BIT;
		if (u.has(Texture::Usage::COLOR_ATTACHMENT)) out |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::DEPTH_STENCIL_ATTACHMENT)) out |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::TRANSIENT_ATTACHMENT)) out |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		if (u.has(Texture::Usage::INPUT_ATTACHMENT)) out |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		
		// VK_KHR_video_decode_queue
		if (u.has(Texture::Usage::VIDEO_DECODE_DST)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_DECODE_SRC)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_DECODE_DPB)) out |= VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR;
		
		// VK_KHR_video_encode_queue
		if (u.has(Texture::Usage::VIDEO_ENCODE_DST)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_ENCODE_SRC)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
		if (u.has(Texture::Usage::VIDEO_ENCODE_DPB)) out |= VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;

		// VK_EXT_fragment_density_map
		if (u.has(Texture::Usage::FRAGMENT_DENSITY_MAP)) out |= VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;

		// VK_KHR_fragment_shading_rate
		if (u.has(Texture::Usage::FRAGMENT_SHADING_RATE_ATTACHMENT)) out |= VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;

		// VK_EXT_host_image_copy
		if (u.has(Texture::Usage::HOST_TRANSFER)) out |= VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;

		// VK_EXT_attachment_feedback_loop_layout
		if (u.has(Texture::Usage::ATTACHMENT_FEEDBACK_LOOP)) out |= VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;

		// VK_HUAWEI_invocation_mask
		if (u.has(Texture::Usage::INVOCATION_MASK)) out |= VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI;

		// VK_QCOM_image_processing
		if (u.has(Texture::Usage::SAMPLE_WEIGHT)) out |= VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM;
		if (u.has(Texture::Usage::SAMPLE_BLOCK_MATCH)) out |= VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM;

		return out;
	}

	constexpr Texture::Usage TextureUsageToRaindrop(const VkImageUsageFlags& flags){
		Texture::Usage out = Texture::Usage::NONE;
		
		if (flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) out.set(Texture::Usage::TRANSFER_SRC);
		if (flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) out.set(Texture::Usage::TRANSFER_DST);
		if (flags & VK_IMAGE_USAGE_SAMPLED_BIT) out.set(Texture::Usage::SAMPLED);
		if (flags & VK_IMAGE_USAGE_STORAGE_BIT) out.set(Texture::Usage::STORAGE);
		if (flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) out.set(Texture::Usage::COLOR_ATTACHMENT);
		if (flags & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) out.set(Texture::Usage::DEPTH_STENCIL_ATTACHMENT);
		if (flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) out.set(Texture::Usage::TRANSIENT_ATTACHMENT);
		if (flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) out.set(Texture::Usage::INPUT_ATTACHMENT);
		if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR) out.set(Texture::Usage::VIDEO_DECODE_DST);
		if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR) out.set(Texture::Usage::VIDEO_DECODE_SRC);
		if (flags & VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR) out.set(Texture::Usage::VIDEO_DECODE_DPB);
		if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR) out.set(Texture::Usage::VIDEO_ENCODE_DST);
		if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR) out.set(Texture::Usage::VIDEO_ENCODE_SRC);
		if (flags & VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR) out.set(Texture::Usage::VIDEO_ENCODE_DPB);
		if (flags & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT) out.set(Texture::Usage::FRAGMENT_DENSITY_MAP);
		if (flags & VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) out.set(Texture::Usage::FRAGMENT_SHADING_RATE_ATTACHMENT);
		if (flags & VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT) out.set(Texture::Usage::HOST_TRANSFER);
		if (flags & VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT) out.set(Texture::Usage::ATTACHMENT_FEEDBACK_LOOP);
		if (flags & VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI) out.set(Texture::Usage::INVOCATION_MASK);
		if (flags & VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM) out.set(Texture::Usage::SAMPLE_WEIGHT);
		if (flags & VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM) out.set(Texture::Usage::SAMPLE_BLOCK_MATCH);

		return out;
	}

	constexpr VkImageUsageFlagBits TextureUsageBitsToVulkan(const Texture::Usage::Bits& u){
		switch (u){
			case Texture::Usage::TRANSFER_SRC: return VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			case Texture::Usage::TRANSFER_DST: return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			case Texture::Usage::SAMPLED: return VK_IMAGE_USAGE_SAMPLED_BIT;
			case Texture::Usage::STORAGE: return VK_IMAGE_USAGE_STORAGE_BIT;
			case Texture::Usage::COLOR_ATTACHMENT: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			case Texture::Usage::DEPTH_STENCIL_ATTACHMENT: return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			case Texture::Usage::TRANSIENT_ATTACHMENT: return VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
			case Texture::Usage::INPUT_ATTACHMENT: return VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
			case Texture::Usage::VIDEO_DECODE_DST: return VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR;
			case Texture::Usage::VIDEO_DECODE_SRC: return VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
			case Texture::Usage::VIDEO_DECODE_DPB: return VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR;
			case Texture::Usage::VIDEO_ENCODE_DST: return VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
			case Texture::Usage::VIDEO_ENCODE_SRC: return VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
			case Texture::Usage::VIDEO_ENCODE_DPB: return VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR;
			case Texture::Usage::FRAGMENT_DENSITY_MAP: return VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;
			case Texture::Usage::FRAGMENT_SHADING_RATE_ATTACHMENT: return VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
			case Texture::Usage::HOST_TRANSFER: return VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT;
			case Texture::Usage::ATTACHMENT_FEEDBACK_LOOP: return VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;
			case Texture::Usage::INVOCATION_MASK: return VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI;
			case Texture::Usage::SAMPLE_WEIGHT: return VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM;
			case Texture::Usage::SAMPLE_BLOCK_MATCH: return VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM;
			default: return VkImageUsageFlagBits(0);
		}
	}

	constexpr Texture::Usage::Bits TextureUsageBitsToRaindrop(const VkImageUsageFlagBits& u){
		switch (u){
			case VK_IMAGE_USAGE_TRANSFER_SRC_BIT: return Texture::Usage::TRANSFER_SRC;
			case VK_IMAGE_USAGE_TRANSFER_DST_BIT: return Texture::Usage::TRANSFER_DST;
			case VK_IMAGE_USAGE_SAMPLED_BIT: return Texture::Usage::SAMPLED;
			case VK_IMAGE_USAGE_STORAGE_BIT: return Texture::Usage::STORAGE;
			case VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT: return Texture::Usage::COLOR_ATTACHMENT;
			case VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT: return Texture::Usage::DEPTH_STENCIL_ATTACHMENT;
			case VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT: return Texture::Usage::TRANSIENT_ATTACHMENT;
			case VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT: return Texture::Usage::INPUT_ATTACHMENT;
			case VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR: return Texture::Usage::VIDEO_DECODE_DST;
			case VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR: return Texture::Usage::VIDEO_DECODE_SRC;
			case VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR: return Texture::Usage::VIDEO_DECODE_DPB;
			case VK_IMAGE_USAGE_VIDEO_ENCODE_DST_BIT_KHR: return Texture::Usage::VIDEO_ENCODE_DST;
			case VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR: return Texture::Usage::VIDEO_ENCODE_SRC;
			case VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR: return Texture::Usage::VIDEO_ENCODE_DPB;
			case VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT: return Texture::Usage::FRAGMENT_DENSITY_MAP;
			case VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR: return Texture::Usage::FRAGMENT_SHADING_RATE_ATTACHMENT;
			case VK_IMAGE_USAGE_HOST_TRANSFER_BIT_EXT: return Texture::Usage::HOST_TRANSFER;
			case VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT: return Texture::Usage::ATTACHMENT_FEEDBACK_LOOP;
			case VK_IMAGE_USAGE_INVOCATION_MASK_BIT_HUAWEI: return Texture::Usage::INVOCATION_MASK;
			case VK_IMAGE_USAGE_SAMPLE_WEIGHT_BIT_QCOM: return Texture::Usage::SAMPLE_WEIGHT;
			case VK_IMAGE_USAGE_SAMPLE_BLOCK_MATCH_BIT_QCOM: return Texture::Usage::SAMPLE_BLOCK_MATCH;
			default: return Texture::Usage::NONE;
		}
	}

	constexpr VkImageType TextureTypeToVulkan(const Texture::Type& t){
		switch (t){
			case Texture::Type::LINEAR: return VK_IMAGE_TYPE_1D;
			case Texture::Type::PLANAR: return VK_IMAGE_TYPE_2D;
			case Texture::Type::VOLUMETRIC: return VK_IMAGE_TYPE_3D;
			default: return VK_IMAGE_TYPE_2D;
		}
	}

	constexpr Texture::Type TextureTypeToRaindrop(const VkImageType& t){
		switch (t){
			case VK_IMAGE_TYPE_1D: return Texture::Type::LINEAR;
			case VK_IMAGE_TYPE_2D: return Texture::Type::PLANAR;
			case VK_IMAGE_TYPE_3D: return Texture::Type::VOLUMETRIC;
			default: return Texture::Type::PLANAR;
		}
	}

	constexpr VkImageTiling TextureTilingToVulkan(const Texture::Tiling& t){
		switch (t){
			case Texture::Tiling::LINEAR: return VK_IMAGE_TILING_LINEAR;
			case Texture::Tiling::OPTIMAL: return VK_IMAGE_TILING_OPTIMAL;
			default: return VK_IMAGE_TILING_OPTIMAL;
		}
	}

	constexpr Texture::Tiling TextureTilingToRaindrop(const VkImageTiling& t){
		switch (t){
			case VK_IMAGE_TILING_LINEAR: return Texture::Tiling::LINEAR;
			case VK_IMAGE_TILING_OPTIMAL: return Texture::Tiling::OPTIMAL;
			default: return Texture::Tiling::OPTIMAL;
		}
	}

	constexpr VkImageLayout TextureLayoutToVulkan(const Texture::Layout& layout){
		switch (layout){
			case Texture::Layout::UNDEFINED: return VK_IMAGE_LAYOUT_UNDEFINED;
			case Texture::Layout::GENERAL: return VK_IMAGE_LAYOUT_GENERAL;
			case Texture::Layout::COLOR_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::SHADER_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case Texture::Layout::TRANSFER_SRC_OPTIMAL: return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case Texture::Layout::TRANSFER_DST_OPTIMAL: return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case Texture::Layout::PREINITIALIZED: return VK_IMAGE_LAYOUT_PREINITIALIZED;
			case Texture::Layout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::DEPTH_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			case Texture::Layout::DEPTH_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
			case Texture::Layout::STENCIL_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
			case Texture::Layout::STENCIL_READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
			case Texture::Layout::READ_ONLY_OPTIMAL: return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
			case Texture::Layout::ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

			// VK_KHR_swapchain
			case Texture::Layout::PRESENT_SRC: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			//VK_KHR_video_decode_queue
			case Texture::Layout::VIDEO_DECODE_DST: return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
			case Texture::Layout::VIDEO_DECODE_SRC: return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
			case Texture::Layout::VIDEO_DECODE_DPB: return VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;

			// VK_KHR_video_encode_queue
			case Texture::Layout::VIDEO_ENCODE_DST: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR;
			case Texture::Layout::VIDEO_ENCODE_SRC: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR;
			case Texture::Layout::VIDEO_ENCODE_DPB: return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR;

			// VK_KHR_shared_presentable_image
			case Texture::Layout::SHARED_PRESENT: return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;

			// VK_EXT_fragment_density_map
			case Texture::Layout::FRAGMENT_DENSITY_MAP_OPTIMAL: return VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;

			// VK_KHR_fragment_shading_rate
			case Texture::Layout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL: return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;

			// VK_EXT_attachment_feedback_loop_layout
			case Texture::Layout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL: return VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;

			default: return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}

	constexpr Texture::Layout TextureLayoutToRaindrop(const VkImageLayout& layout){
		switch (layout){
			case VK_IMAGE_LAYOUT_UNDEFINED: return Texture::Layout::UNDEFINED;
			case VK_IMAGE_LAYOUT_GENERAL: return Texture::Layout::GENERAL;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return Texture::Layout::COLOR_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return Texture::Layout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return Texture::Layout::DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return Texture::Layout::SHADER_READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: return Texture::Layout::TRANSFER_SRC_OPTIMAL;
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: return Texture::Layout::TRANSFER_DST_OPTIMAL;
			case VK_IMAGE_LAYOUT_PREINITIALIZED: return Texture::Layout::PREINITIALIZED;
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: return Texture::Layout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: return Texture::Layout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL: return Texture::Layout::DEPTH_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL: return Texture::Layout::DEPTH_READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL: return Texture::Layout::STENCIL_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL: return Texture::Layout::STENCIL_READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL: return Texture::Layout::READ_ONLY_OPTIMAL;
			case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL: return Texture::Layout::ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: return Texture::Layout::PRESENT_SRC;
			case VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR: return Texture::Layout::VIDEO_DECODE_DST;
			case VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR: return Texture::Layout::VIDEO_DECODE_SRC;
			case VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR: return Texture::Layout::VIDEO_DECODE_DPB;
			case VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR: return Texture::Layout::VIDEO_ENCODE_DST;
			case VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR: return Texture::Layout::VIDEO_ENCODE_SRC;
			case VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR: return Texture::Layout::VIDEO_ENCODE_DPB;
			case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR: return Texture::Layout::SHARED_PRESENT;
			case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT: return Texture::Layout::FRAGMENT_DENSITY_MAP_OPTIMAL;
			case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR: return Texture::Layout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL;
			case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT: return Texture::Layout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL;
			default: return Texture::Layout::UNDEFINED;
		}
	}
}

#endif