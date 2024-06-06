#ifndef __RAINDROP_INTERNAL_TEXTURE_SUBSET_HPP__
#define __RAINDROP_INTERNAL_TEXTURE_SUBSET_HPP__

#include <Raindrop/TextureSubset.hpp>
#include <Raindrop_internal/Graphics/ImageView.hpp>

namespace Raindrop{
	struct TextureSubset::Impl{
		struct Builder{
			Internal::Graphics::ImageViewConfigInfo info;
		};

		std::shared_ptr<Internal::Graphics::ImageView> imageView;
		std::unique_ptr<Builder> builder;
		Context* context;

		Impl() :
			imageView{nullptr},
			builder{nullptr},
			context{nullptr}
		{}
	};

	constexpr VkImageViewType TextureSubsetTypeToVulkan(const TextureSubset::Type& type){
		switch (type){
			case TextureSubset::Type::LINEAR: return VK_IMAGE_VIEW_TYPE_1D;
			case TextureSubset::Type::PLANAR: return VK_IMAGE_VIEW_TYPE_2D;
			case TextureSubset::Type::VOLUMETRIC: return VK_IMAGE_VIEW_TYPE_3D;
			case TextureSubset::Type::CUBEMAP: return VK_IMAGE_VIEW_TYPE_CUBE;
			case TextureSubset::Type::LINEAR_ARRAY: return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case TextureSubset::Type::PLANAR_ARRAY: return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			case TextureSubset::Type::CUBEMAP_ARRAY: return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			default: return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	constexpr TextureSubset::Type TextureSubsetTypeToRaindrop(const VkImageViewType& type){
		switch (type){
			case VK_IMAGE_VIEW_TYPE_1D: return TextureSubset::Type::LINEAR;
			case VK_IMAGE_VIEW_TYPE_2D: return TextureSubset::Type::PLANAR;
			case VK_IMAGE_VIEW_TYPE_3D: return TextureSubset::Type::VOLUMETRIC;
			case VK_IMAGE_VIEW_TYPE_CUBE: return TextureSubset::Type::CUBEMAP;
			case VK_IMAGE_VIEW_TYPE_1D_ARRAY: return TextureSubset::Type::LINEAR_ARRAY;
			case VK_IMAGE_VIEW_TYPE_2D_ARRAY: return TextureSubset::Type::PLANAR_ARRAY;
			case VK_IMAGE_VIEW_TYPE_CUBE_ARRAY: return TextureSubset::Type::CUBEMAP_ARRAY;
			default: return TextureSubset::Type::PLANAR;
		}
	}

	constexpr VkImageViewCreateFlags TextureSubsetFlagsToVulkan(const TextureSubset::Flags& flags){
		VkImageViewCreateFlags out = 0;

		// VK_EXT_fragment_density_map
		if (flags.has(TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DYNAMIC)) out |= VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT;

		// VK_EXT_fragment_density_map2
		if (flags.has(TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DEFERRED)) out |= VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT;

		// VK_EXT_descriptor_buffer
		if (flags.has(TextureSubset::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY)) out |= VK_IMAGE_VIEW_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;

		return out;
	}

	constexpr TextureSubset::Flags TextureSubsetFlagsToRaindrop(const VkImageViewCreateFlags& flags){
		TextureSubset::Flags out = TextureSubset::Flags::NONE;

		if (flags & VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT) out |= TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DYNAMIC;
		if (flags & VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT) out |= TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DEFERRED;
		if (flags & VK_IMAGE_VIEW_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT) out |= TextureSubset::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY;

		return out;
	}

	constexpr VkImageViewCreateFlagBits TextureSubsetFlagsBitsToVulkan(const TextureSubset::Flags::Bits& flags){
		switch (flags){
			case TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DYNAMIC: return VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT;
			case TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DEFERRED: return VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT;
			case TextureSubset::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY: return VK_IMAGE_VIEW_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;
			default: return VkImageViewCreateFlagBits(0);
		}
	}

	constexpr TextureSubset::Flags::Bits TextureSubsetFlagsBitsToRaindrop(const VkImageViewCreateFlagBits& flags){
		switch (flags){
			case VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT: return TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DYNAMIC;
			case VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT: return TextureSubset::Flags::FRAGMENT_DENSITY_MAP_DEFERRED;
			case VK_IMAGE_VIEW_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT: return TextureSubset::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY;
			default: return TextureSubset::Flags::NONE;
		}
	}
}

#endif