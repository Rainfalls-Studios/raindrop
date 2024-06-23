#ifndef __RAINDROP_INTERNAL_GBUFFER_HPP__
#define __RAINDROP_INTERNAL_GBUFFER_HPP__

#include <Raindrop/GBuffer.hpp>
#include <Raindrop_internal/Graphics/Buffer.hpp>
#include <Raindrop_internal/Context.hpp>

namespace Raindrop{
	struct GBuffer::Impl{
		Internal::Context* context;

		std::size_t size;
		Usage usage;
		Flags flags;
		GMemory::Type::Flags memoryFlags;
		std::size_t alignement;

		std::size_t mappedSize;
		std::size_t mappedOffset;
		void* mappedPointer;

		std::shared_ptr<Internal::Graphics::Buffer> internal;

		Impl() : 
			context{nullptr},
			size{0},
			usage{Usage::NONE},
			flags{Flags::NONE},
			memoryFlags{GMemory::Type::Flags::NONE},
			alignement{0},
			mappedSize{0},
			mappedOffset{0},
			mappedPointer{nullptr},
			internal{nullptr}
		{}
	};

	constexpr VkBufferUsageFlags GBufferUsageFlagsToVulkan(const GBuffer::Usage& usage) noexcept{
		VkBufferUsageFlags out = 0;

		if (usage.has(GBuffer::Usage::TRANSFER_SRC)) out |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		if (usage.has(GBuffer::Usage::TRANSFER_DST)) out |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if (usage.has(GBuffer::Usage::UNIFORM_TEXEL_BUFFER)) out |= VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::STORAGE_TEXEL_BUFFER)) out |= VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::UNIFORM_BUFFER)) out |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::STORAGE_BUFFER)) out |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::INDEX_BUFFER)) out |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::VERTEX_BUFFER)) out |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if (usage.has(GBuffer::Usage::INDIRECT_BUFFER)) out |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

		// VK_VERSION_1_2
		if (usage.has(GBuffer::Usage::SHADER_DEVICE_ADDRESS)) out |= VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

		// VK_KHR_video_decode_queue
		if (usage.has(GBuffer::Usage::VIDEO_DECODE_SRC)) out |= VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
		if (usage.has(GBuffer::Usage::VIDEO_DECODE_DST)) out |= VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR;

		// VK_EXT_transform_feedback
		if (usage.has(GBuffer::Usage::TRANSFORM_FEEDBACK_BUFFER)) out |= VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT;
		if (usage.has(GBuffer::Usage::TRANSFORM_FEEDBACK_COUNTER_BUFFER)) out |= VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT;

		// VK_EXT_conditional_rendering
		if (usage.has(GBuffer::Usage::CONDITIONAL_RENDERING)) out |= VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT;

		// VK_KHR_acceleration_structure
		if (usage.has(GBuffer::Usage::ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY)) out |= VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
		if (usage.has(GBuffer::Usage::ACCELERATION_STRUCTURE_STORAGE)) out |= VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;

		// VK_KHR_ray_tracing_pipeline
		if (usage.has(GBuffer::Usage::SHADER_BINDING_TABLE)) out |= VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR;

		// VK_KHR_video_encode_queue
		if (usage.has(GBuffer::Usage::VIDEO_ENCODE_DST)) out |= VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
		if (usage.has(GBuffer::Usage::VIDEO_ENCODE_SRC)) out |= VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;

		// VK_EXT_descriptor_buffer
		if (usage.has(GBuffer::Usage::SAMPLER_DESCRIPTOR_BUFFER)) out |= VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT;
		if (usage.has(GBuffer::Usage::RESOURCE_DESCRIPTOR_BUFFER)) out |= VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
		if (usage.has(GBuffer::Usage::PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER)) out |= VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT;

		// VK_EXT_opacity_micromap
		if (usage.has(GBuffer::Usage::MICROMAP_BUILD_INPUT_READ_ONLY)) out |= VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT;
		if (usage.has(GBuffer::Usage::MICROMAP_STORAGE)) out |= VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT;

		return out;
	}

	constexpr GBuffer::Usage GBufferUsageFlagsToRaindrop(const VkBufferUsageFlags& usage) noexcept{
		GBuffer::Usage out = GBuffer::Usage::NONE;

		if (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) out |= GBuffer::Usage::TRANSFER_SRC;
		if (usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) out |= GBuffer::Usage::TRANSFER_DST;
		if (usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT) out |= GBuffer::Usage::UNIFORM_TEXEL_BUFFER;
		if (usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT) out |= GBuffer::Usage::STORAGE_TEXEL_BUFFER;
		if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) out |= GBuffer::Usage::UNIFORM_BUFFER;
		if (usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) out |= GBuffer::Usage::STORAGE_BUFFER;
		if (usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) out |= GBuffer::Usage::INDEX_BUFFER;
		if (usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) out |= GBuffer::Usage::VERTEX_BUFFER;
		if (usage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT) out |= GBuffer::Usage::INDIRECT_BUFFER;
		if (usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) out |= GBuffer::Usage::SHADER_DEVICE_ADDRESS;
		if (usage & VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR) out |= GBuffer::Usage::VIDEO_DECODE_SRC;
		if (usage & VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR) out |= GBuffer::Usage::VIDEO_DECODE_DST;
		if (usage & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT) out |= GBuffer::Usage::TRANSFORM_FEEDBACK_BUFFER;
		if (usage & VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT) out |= GBuffer::Usage::TRANSFORM_FEEDBACK_COUNTER_BUFFER;
		if (usage & VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT) out |= GBuffer::Usage::CONDITIONAL_RENDERING;
		if (usage & VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR) out |= GBuffer::Usage::ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY;
		if (usage & VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR) out |= GBuffer::Usage::ACCELERATION_STRUCTURE_STORAGE;
		if (usage & VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR) out |= GBuffer::Usage::SHADER_BINDING_TABLE;
		if (usage & VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR) out |= GBuffer::Usage::VIDEO_ENCODE_DST;
		if (usage & VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR) out |= GBuffer::Usage::VIDEO_ENCODE_SRC;
		if (usage & VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT) out |= GBuffer::Usage::SAMPLER_DESCRIPTOR_BUFFER;
		if (usage & VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT) out |= GBuffer::Usage::RESOURCE_DESCRIPTOR_BUFFER;
		if (usage & VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT) out |= GBuffer::Usage::PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER;
		if (usage & VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT) out |= GBuffer::Usage::MICROMAP_BUILD_INPUT_READ_ONLY;
		if (usage & VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT) out |= GBuffer::Usage::MICROMAP_STORAGE;

		return out;
	}

	constexpr VkBufferUsageFlagBits GBufferUsageFlagsBitsToVulkan(const GBuffer::Usage::Bits& usage) noexcept{
		switch (usage){
			case GBuffer::Usage::TRANSFER_SRC: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			case GBuffer::Usage::TRANSFER_DST: return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
			case GBuffer::Usage::UNIFORM_TEXEL_BUFFER: return VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT;
			case GBuffer::Usage::STORAGE_TEXEL_BUFFER: return VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT;
			case GBuffer::Usage::UNIFORM_BUFFER: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			case GBuffer::Usage::STORAGE_BUFFER: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
			case GBuffer::Usage::INDEX_BUFFER: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			case GBuffer::Usage::VERTEX_BUFFER: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			case GBuffer::Usage::INDIRECT_BUFFER: return VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
			case GBuffer::Usage::SHADER_DEVICE_ADDRESS: return VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
			case GBuffer::Usage::VIDEO_DECODE_SRC: return VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR;
			case GBuffer::Usage::VIDEO_DECODE_DST: return VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR;
			case GBuffer::Usage::TRANSFORM_FEEDBACK_BUFFER: return VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT;
			case GBuffer::Usage::TRANSFORM_FEEDBACK_COUNTER_BUFFER: return VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT;
			case GBuffer::Usage::CONDITIONAL_RENDERING: return VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT;
			case GBuffer::Usage::ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY: return VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;
			case GBuffer::Usage::ACCELERATION_STRUCTURE_STORAGE: return VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;
			case GBuffer::Usage::SHADER_BINDING_TABLE: return VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR;
			case GBuffer::Usage::VIDEO_ENCODE_DST: return VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR;
			case GBuffer::Usage::VIDEO_ENCODE_SRC: return VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR;
			case GBuffer::Usage::SAMPLER_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT;
			case GBuffer::Usage::RESOURCE_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
			case GBuffer::Usage::PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER: return VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT;
			case GBuffer::Usage::MICROMAP_BUILD_INPUT_READ_ONLY: return VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT;
			case GBuffer::Usage::MICROMAP_STORAGE: return VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT;
			default: return VkBufferUsageFlagBits(0);
		}
	}

	constexpr GBuffer::Usage::Bits GBufferUsageFlagsBitsToRaindrop(const VkBufferUsageFlagBits& usage) noexcept{
		switch (usage){
			case VK_BUFFER_USAGE_TRANSFER_SRC_BIT: return GBuffer::Usage::TRANSFER_SRC;
			case VK_BUFFER_USAGE_TRANSFER_DST_BIT: return GBuffer::Usage::TRANSFER_DST;
			case VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT: return GBuffer::Usage::UNIFORM_TEXEL_BUFFER;
			case VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT: return GBuffer::Usage::STORAGE_TEXEL_BUFFER;
			case VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT: return GBuffer::Usage::UNIFORM_BUFFER;
			case VK_BUFFER_USAGE_STORAGE_BUFFER_BIT: return GBuffer::Usage::STORAGE_BUFFER;
			case VK_BUFFER_USAGE_INDEX_BUFFER_BIT: return GBuffer::Usage::INDEX_BUFFER;
			case VK_BUFFER_USAGE_VERTEX_BUFFER_BIT: return GBuffer::Usage::VERTEX_BUFFER;
			case VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT: return GBuffer::Usage::INDIRECT_BUFFER;
			case VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT: return GBuffer::Usage::SHADER_DEVICE_ADDRESS;
			case VK_BUFFER_USAGE_VIDEO_DECODE_SRC_BIT_KHR: return GBuffer::Usage::VIDEO_DECODE_SRC;
			case VK_BUFFER_USAGE_VIDEO_DECODE_DST_BIT_KHR: return GBuffer::Usage::VIDEO_DECODE_DST;
			case VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT: return GBuffer::Usage::TRANSFORM_FEEDBACK_BUFFER;
			case VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT: return GBuffer::Usage::TRANSFORM_FEEDBACK_COUNTER_BUFFER;
			case VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT: return GBuffer::Usage::CONDITIONAL_RENDERING;
			case VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR: return GBuffer::Usage::ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY;
			case VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR: return GBuffer::Usage::ACCELERATION_STRUCTURE_STORAGE;
			case VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR: return GBuffer::Usage::SHADER_BINDING_TABLE;
			case VK_BUFFER_USAGE_VIDEO_ENCODE_DST_BIT_KHR: return GBuffer::Usage::VIDEO_ENCODE_DST;
			case VK_BUFFER_USAGE_VIDEO_ENCODE_SRC_BIT_KHR: return GBuffer::Usage::VIDEO_ENCODE_SRC;
			case VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT: return GBuffer::Usage::SAMPLER_DESCRIPTOR_BUFFER;
			case VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT: return GBuffer::Usage::RESOURCE_DESCRIPTOR_BUFFER;
			case VK_BUFFER_USAGE_PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER_BIT_EXT: return GBuffer::Usage::PUSH_DESCRIPTORS_DESCRIPTOR_BUFFER;
			case VK_BUFFER_USAGE_MICROMAP_BUILD_INPUT_READ_ONLY_BIT_EXT: return GBuffer::Usage::MICROMAP_BUILD_INPUT_READ_ONLY;
			case VK_BUFFER_USAGE_MICROMAP_STORAGE_BIT_EXT: return GBuffer::Usage::MICROMAP_STORAGE;
			default: return GBuffer::Usage::NONE;
		}
	}

	constexpr VkBufferCreateFlags GBufferFlagsToVulkan(const GBuffer::Flags& flags){
		VkBufferCreateFlags out = 0;

		if (flags.has(GBuffer::Flags::SPARSE_BINDING)) out |= VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
		if (flags.has(GBuffer::Flags::SPARSE_RESIDENCY)) out |= VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT;
		if (flags.has(GBuffer::Flags::SPARSE_ALIASED)) out |= VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;

		// Provided by VK_VERSION_1_1
		if (flags.has(GBuffer::Flags::PROTECTED)) out |= VK_BUFFER_CREATE_PROTECTED_BIT;

		// Provided by VK_VERSION_1_2
		if (flags.has(GBuffer::Flags::DEVICE_ADDRESS_CAPTURE_REPLAY)) out |= VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT;

		// Provided by VK_EXT_descriptor_buffer
		if (flags.has(GBuffer::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY)) out |= VK_BUFFER_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;

		// Provided by VK_KHR_video_maintenance1
		if (flags.has(GBuffer::Flags::VIDEO_PROFILE_INDEPENDENT)) out |= VK_BUFFER_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR;

		return out;
	}

	constexpr GBuffer::Flags GBufferFlagsToRaindrop(const VkBufferCreateFlags& flags){
		GBuffer::Flags out = GBuffer::Flags::NONE;

		if (flags & VK_BUFFER_CREATE_SPARSE_BINDING_BIT) out |= GBuffer::Flags::SPARSE_BINDING;
		if (flags & VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT) out |= GBuffer::Flags::SPARSE_RESIDENCY;
		if (flags & VK_BUFFER_CREATE_SPARSE_ALIASED_BIT) out |= GBuffer::Flags::SPARSE_ALIASED;
		if (flags & VK_BUFFER_CREATE_PROTECTED_BIT) out |= GBuffer::Flags::PROTECTED;
		if (flags & VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT) out |= GBuffer::Flags::DEVICE_ADDRESS_CAPTURE_REPLAY;
		if (flags & VK_BUFFER_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT) out |= GBuffer::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY;
		if (flags & VK_BUFFER_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR) out |= GBuffer::Flags::VIDEO_PROFILE_INDEPENDENT;

		return out;
	}

	constexpr GBuffer::Flags::Bits GBufferFlagsBitsToRaindrop(const VkBufferCreateFlagBits& flags){
		switch (flags){
			case VK_BUFFER_CREATE_SPARSE_BINDING_BIT: return GBuffer::Flags::SPARSE_BINDING;
			case VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT: return GBuffer::Flags::SPARSE_RESIDENCY;
			case VK_BUFFER_CREATE_SPARSE_ALIASED_BIT: return GBuffer::Flags::SPARSE_ALIASED;
			case VK_BUFFER_CREATE_PROTECTED_BIT: return GBuffer::Flags::PROTECTED;
			case VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT: return GBuffer::Flags::DEVICE_ADDRESS_CAPTURE_REPLAY;
			case VK_BUFFER_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT: return GBuffer::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY;
			case VK_BUFFER_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR: return GBuffer::Flags::VIDEO_PROFILE_INDEPENDENT;
			default: return GBuffer::Flags::NONE;
		}
	}

	constexpr VkBufferCreateFlagBits GBufferFlagsBitsToVulkan(const GBuffer::Flags::Bits& flags){
		switch (flags){
			case GBuffer::Flags::SPARSE_BINDING: return VK_BUFFER_CREATE_SPARSE_BINDING_BIT;
			case GBuffer::Flags::SPARSE_RESIDENCY: return VK_BUFFER_CREATE_SPARSE_RESIDENCY_BIT;
			case GBuffer::Flags::SPARSE_ALIASED: return VK_BUFFER_CREATE_SPARSE_ALIASED_BIT;
			case GBuffer::Flags::PROTECTED: return VK_BUFFER_CREATE_PROTECTED_BIT;
			case GBuffer::Flags::DEVICE_ADDRESS_CAPTURE_REPLAY: return VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT;
			case GBuffer::Flags::DESCRIPTOR_BUFFER_CAPTURE_REPLAY: return VK_BUFFER_CREATE_DESCRIPTOR_BUFFER_CAPTURE_REPLAY_BIT_EXT;
			case GBuffer::Flags::VIDEO_PROFILE_INDEPENDENT: return VK_BUFFER_CREATE_VIDEO_PROFILE_INDEPENDENT_BIT_KHR;
			default: return VkBufferCreateFlagBits(0);
		}
	}
}

#endif