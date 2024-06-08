#ifndef __RAINDROP_INTERNAL_RENDER_PASS_HPP__
#define __RAINDROP_INTERNAL_RENDER_PASS_HPP__

#include <Raindrop/RenderPass.hpp>
#include <Raindrop_internal/Graphics/RenderPass.hpp>

namespace Raindrop{
	struct RenderPass::Impl{
		struct Builder{
			Internal::Graphics::RenderPassConfigInfo info;

			struct {
				std::vector<VkAttachmentDescription> attachments;
				std::vector<VkSubpassDescription> subpasses;
				std::vector<VkSubpassDependency> dependencies;
				
				std::vector<uint32_t> preserveAttachments;
				std::vector<VkAttachmentReference> references;
				std::list<VkAttachmentReference> depthStencils;
			} translation;

			std::vector<Attachment> attachments;
			std::vector<Subpass> subpasses;
			std::vector<Dependency> dependencies;

			void update();
			Builder();
		};

		std::shared_ptr<Internal::Graphics::RenderPass> renderPass;
		std::unique_ptr<Builder> builder;
		Context* context;

		Impl() : 
			renderPass{nullptr},
			builder{nullptr},
			context{nullptr}
		{}
	};

	constexpr VkAccessFlags AccessFlagsToVulkan(const RenderPass::Access& access){
		VkAccessFlags out = 0;

		if (access.has(RenderPass::Access::INDIRECT_COMMAND_READ)) out |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
		if (access.has(RenderPass::Access::INDEX_READ)) out |= VK_ACCESS_INDEX_READ_BIT;
		if (access.has(RenderPass::Access::VERTEX_ATTRIBUTE_READ)) out |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
		if (access.has(RenderPass::Access::UNIFORM_READ)) out |= VK_ACCESS_UNIFORM_READ_BIT;
		if (access.has(RenderPass::Access::INPUT_ATTACHMENT_READ)) out |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
		if (access.has(RenderPass::Access::SHADER_READ)) out |= VK_ACCESS_SHADER_READ_BIT;
		if (access.has(RenderPass::Access::SHADER_WRITE)) out |= VK_ACCESS_SHADER_WRITE_BIT;
		if (access.has(RenderPass::Access::COLOR_ATTACHMENT_READ)) out |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		if (access.has(RenderPass::Access::COLOR_ATTACHMENT_WRITE)) out |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		if (access.has(RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_READ)) out |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
		if (access.has(RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_WRITE)) out |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		if (access.has(RenderPass::Access::TRANSFER_READ)) out |= VK_ACCESS_TRANSFER_READ_BIT;
		if (access.has(RenderPass::Access::TRANSFER_WRITE)) out |= VK_ACCESS_TRANSFER_WRITE_BIT;
		if (access.has(RenderPass::Access::HOST_READ)) out |= VK_ACCESS_HOST_READ_BIT;
		if (access.has(RenderPass::Access::HOST_WRITE)) out |= VK_ACCESS_HOST_WRITE_BIT;
		if (access.has(RenderPass::Access::MEMORY_READ)) out |= VK_ACCESS_MEMORY_READ_BIT;
		if (access.has(RenderPass::Access::MEMORY_WRITE)) out |= VK_ACCESS_MEMORY_WRITE_BIT;
		
		// VK_EXT_transform_feedback
		if (access.has(RenderPass::Access::TRANSFORM_FEEDBACK_WRITE)) out |= VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
		if (access.has(RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_READ)) out |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
		if (access.has(RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_WRITE)) out |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;

		// VK_EXT_conditional_rendering
		if (access.has(RenderPass::Access::CONDITIONAL_RENDERING_READ)) out |= VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;

		// VK_EXT_blend_operation_advanced
		if (access.has(RenderPass::Access::COLOR_ATTACHMENT_READ_NONCOHERENT)) out |= VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;

		// VK_KHR_acceleration_structure
		if (access.has(RenderPass::Access::ACCELERATION_STRUCTURE_READ)) out |= VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
		if (access.has(RenderPass::Access::ACCELERATION_STRUCTURE_WRITE)) out |= VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;

		// VK_EXT_fragment_density_map
		if (access.has(RenderPass::Access::FRAGMENT_DENSITY_MAP_READ)) out |= VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;

		// VK_KHR_fragment_shading_rate
		if (access.has(RenderPass::Access::FRAGMENT_SHADING_RATE_ATTACHMENT_READ)) out |= VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;

		// VK_NV_device_generated_commands
		if (access.has(RenderPass::Access::COMMAND_PREPROCESS_READ)) out |= VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV;
		if (access.has(RenderPass::Access::COMMAND_PREPROCESS_WRITE)) out |= VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;

		return out;
	}

	constexpr RenderPass::Access AccessFlagsToRaindrop(const VkAccessFlags& access){
		RenderPass::Access out = RenderPass::Access::NONE;

		if (access & VK_ACCESS_INDIRECT_COMMAND_READ_BIT) out |= RenderPass::Access::INDIRECT_COMMAND_READ;
		if (access & VK_ACCESS_INDEX_READ_BIT) out |= RenderPass::Access::INDEX_READ;
		if (access & VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT) out |= RenderPass::Access::VERTEX_ATTRIBUTE_READ;
		if (access & VK_ACCESS_UNIFORM_READ_BIT) out |= RenderPass::Access::UNIFORM_READ;
		if (access & VK_ACCESS_INPUT_ATTACHMENT_READ_BIT) out |= RenderPass::Access::INPUT_ATTACHMENT_READ;
		if (access & VK_ACCESS_SHADER_READ_BIT) out |= RenderPass::Access::SHADER_READ;
		if (access & VK_ACCESS_SHADER_WRITE_BIT) out |= RenderPass::Access::SHADER_WRITE;
		if (access & VK_ACCESS_COLOR_ATTACHMENT_READ_BIT) out |= RenderPass::Access::COLOR_ATTACHMENT_READ;
		if (access & VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT) out |= RenderPass::Access::COLOR_ATTACHMENT_WRITE;
		if (access & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT) out |= RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_READ;
		if (access & VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT) out |= RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_WRITE;
		if (access & VK_ACCESS_TRANSFER_READ_BIT) out |= RenderPass::Access::TRANSFER_READ;
		if (access & VK_ACCESS_TRANSFER_WRITE_BIT) out |= RenderPass::Access::TRANSFER_WRITE;
		if (access & VK_ACCESS_HOST_READ_BIT) out |= RenderPass::Access::HOST_READ;
		if (access & VK_ACCESS_HOST_WRITE_BIT) out |= RenderPass::Access::HOST_WRITE;
		if (access & VK_ACCESS_MEMORY_READ_BIT) out |= RenderPass::Access::MEMORY_READ;
		if (access & VK_ACCESS_MEMORY_WRITE_BIT) out |= RenderPass::Access::MEMORY_WRITE;
		if (access & VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT) out |= RenderPass::Access::TRANSFORM_FEEDBACK_WRITE;
		if (access & VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT) out |= RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_READ;
		if (access & VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT) out |= RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_WRITE;
		if (access & VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT) out |= RenderPass::Access::CONDITIONAL_RENDERING_READ;
		if (access & VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT) out |= RenderPass::Access::COLOR_ATTACHMENT_READ_NONCOHERENT;
		if (access & VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR) out |= RenderPass::Access::ACCELERATION_STRUCTURE_READ;
		if (access & VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR) out |= RenderPass::Access::ACCELERATION_STRUCTURE_WRITE;
		if (access & VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT) out |= RenderPass::Access::FRAGMENT_DENSITY_MAP_READ;
		if (access & VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR) out |= RenderPass::Access::FRAGMENT_SHADING_RATE_ATTACHMENT_READ;
		if (access & VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV) out |= RenderPass::Access::COMMAND_PREPROCESS_READ;
		if (access & VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV) out |= RenderPass::Access::COMMAND_PREPROCESS_WRITE;

		return out;
	}

	constexpr VkAccessFlagBits AccessFlagsBitsToVulkan(const RenderPass::Access::Bits& access){
		switch (access){
			case RenderPass::Access::INDIRECT_COMMAND_READ: return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
			case RenderPass::Access::INDEX_READ: return VK_ACCESS_INDEX_READ_BIT;
			case RenderPass::Access::VERTEX_ATTRIBUTE_READ: return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
			case RenderPass::Access::UNIFORM_READ: return VK_ACCESS_UNIFORM_READ_BIT;
			case RenderPass::Access::INPUT_ATTACHMENT_READ: return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
			case RenderPass::Access::SHADER_READ: return VK_ACCESS_SHADER_READ_BIT;
			case RenderPass::Access::SHADER_WRITE: return VK_ACCESS_SHADER_WRITE_BIT;
			case RenderPass::Access::COLOR_ATTACHMENT_READ: return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			case RenderPass::Access::COLOR_ATTACHMENT_WRITE: return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			case RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_READ: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			case RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_WRITE: return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			case RenderPass::Access::TRANSFER_READ: return VK_ACCESS_TRANSFER_READ_BIT;
			case RenderPass::Access::TRANSFER_WRITE: return VK_ACCESS_TRANSFER_WRITE_BIT;
			case RenderPass::Access::HOST_READ: return VK_ACCESS_HOST_READ_BIT;
			case RenderPass::Access::HOST_WRITE: return VK_ACCESS_HOST_WRITE_BIT;
			case RenderPass::Access::MEMORY_READ: return VK_ACCESS_MEMORY_READ_BIT;
			case RenderPass::Access::MEMORY_WRITE: return VK_ACCESS_MEMORY_WRITE_BIT;
			case RenderPass::Access::TRANSFORM_FEEDBACK_WRITE: return VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT;
			case RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_READ: return VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT;
			case RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_WRITE: return VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT;
			case RenderPass::Access::CONDITIONAL_RENDERING_READ: return VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT;
			case RenderPass::Access::COLOR_ATTACHMENT_READ_NONCOHERENT: return VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT;
			case RenderPass::Access::ACCELERATION_STRUCTURE_READ: return VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR;
			case RenderPass::Access::ACCELERATION_STRUCTURE_WRITE: return VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR;
			case RenderPass::Access::FRAGMENT_DENSITY_MAP_READ: return VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT;
			case RenderPass::Access::FRAGMENT_SHADING_RATE_ATTACHMENT_READ: return VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR;
			case RenderPass::Access::COMMAND_PREPROCESS_READ: return VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV;
			case RenderPass::Access::COMMAND_PREPROCESS_WRITE: return VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV;
			default: return VkAccessFlagBits(0);
		}
	}

	constexpr RenderPass::Access::Bits AccessFlagsBitsToVulkan(const VkAccessFlagBits& access){
		switch (access){
			case VK_ACCESS_INDIRECT_COMMAND_READ_BIT: return RenderPass::Access::INDIRECT_COMMAND_READ;
			case VK_ACCESS_INDEX_READ_BIT: return RenderPass::Access::INDEX_READ;
			case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT: return RenderPass::Access::VERTEX_ATTRIBUTE_READ;
			case VK_ACCESS_UNIFORM_READ_BIT: return RenderPass::Access::UNIFORM_READ;
			case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT: return RenderPass::Access::INPUT_ATTACHMENT_READ;
			case VK_ACCESS_SHADER_READ_BIT: return RenderPass::Access::SHADER_READ;
			case VK_ACCESS_SHADER_WRITE_BIT: return RenderPass::Access::SHADER_WRITE;
			case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT: return RenderPass::Access::COLOR_ATTACHMENT_READ;
			case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT: return RenderPass::Access::COLOR_ATTACHMENT_WRITE;
			case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT: return RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_READ;
			case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT: return RenderPass::Access::DEPTH_STENCIL_ATTACHMENT_WRITE;
			case VK_ACCESS_TRANSFER_READ_BIT: return RenderPass::Access::TRANSFER_READ;
			case VK_ACCESS_TRANSFER_WRITE_BIT: return RenderPass::Access::TRANSFER_WRITE;
			case VK_ACCESS_HOST_READ_BIT: return RenderPass::Access::HOST_READ;
			case VK_ACCESS_HOST_WRITE_BIT: return RenderPass::Access::HOST_WRITE;
			case VK_ACCESS_MEMORY_READ_BIT: return RenderPass::Access::MEMORY_READ;
			case VK_ACCESS_MEMORY_WRITE_BIT: return RenderPass::Access::MEMORY_WRITE;
			case VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT: return RenderPass::Access::TRANSFORM_FEEDBACK_WRITE;
			case VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT: return RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_READ;
			case VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT: return RenderPass::Access::TRANSFORM_FEEDBACK_COUNTER_WRITE;
			case VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT: return RenderPass::Access::CONDITIONAL_RENDERING_READ;
			case VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT: return RenderPass::Access::COLOR_ATTACHMENT_READ_NONCOHERENT;
			case VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR: return RenderPass::Access::ACCELERATION_STRUCTURE_READ;
			case VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR: return RenderPass::Access::ACCELERATION_STRUCTURE_WRITE;
			case VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT: return RenderPass::Access::FRAGMENT_DENSITY_MAP_READ;
			case VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR: return RenderPass::Access::FRAGMENT_SHADING_RATE_ATTACHMENT_READ;
			case VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_NV: return RenderPass::Access::COMMAND_PREPROCESS_READ;
			case VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_NV: return RenderPass::Access::COMMAND_PREPROCESS_WRITE;
			default: return RenderPass::Access::NONE;
		}
	}

	constexpr VkRenderPassCreateFlags RenderPassFlagsToVulkan(const RenderPass::Flags& flags){
		return VkRenderPassCreateFlags(0);
	}

	constexpr RenderPass::Flags RenderPassFlagsToRaindrop(const VkRenderPassCreateFlags& flags){
		return RenderPass::Flags::NONE;
	}

	constexpr VkRenderPassCreateFlagBits RenderPassFlagsBitsToVulkan(const RenderPass::Flags::Bits& flags){
		return VkRenderPassCreateFlagBits(0);
	}

	constexpr RenderPass::Flags::Bits RenderPassFlagsBitsToRaindrop(const VkRenderPassCreateFlagBits& flags){
		return RenderPass::Flags::NONE;
	}

	constexpr VkAttachmentDescriptionFlags AttachmentFlagsToVulkan(const RenderPass::Attachment::Flags& flags){
		VkAttachmentDescriptionFlags out = 0;

		if (flags.has(RenderPass::Attachment::Flags::MAY_ALIAS)) out |= VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

		return out;
	}

	constexpr RenderPass::Attachment::Flags AttachmentFlagsToRaindrop(const VkAttachmentDescriptionFlags& flags){
		RenderPass::Attachment::Flags out = RenderPass::Attachment::Flags::NONE;

		if (flags & VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT) out |= RenderPass::Attachment::Flags::MAY_ALIAS;

		return out;
	}

	constexpr VkAttachmentDescriptionFlagBits AttachmentFlagsBitsToVulkan(const RenderPass::Attachment::Flags::Bits& flags){
		switch (flags){
			case RenderPass::Attachment::Flags::MAY_ALIAS: return VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
			default: return VkAttachmentDescriptionFlagBits(0);
		}
	}

	constexpr RenderPass::Attachment::Flags::Bits AttachmentFlagsBitsToVulkan(const VkAttachmentDescriptionFlagBits& flags){
		switch (flags){
			case VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT: return RenderPass::Attachment::Flags::MAY_ALIAS;
			default: return RenderPass::Attachment::Flags::NONE;
		}
	}

	constexpr VkAttachmentLoadOp AttachmentLoadOperationToVulkan(const RenderPass::Attachment::Operation::Load& op){
		switch (op){
			case RenderPass::Attachment::Operation::LOAD: return VK_ATTACHMENT_LOAD_OP_LOAD;
			case RenderPass::Attachment::Operation::CLEAR: return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case RenderPass::Attachment::Operation::DONT_LOAD: return VK_ATTACHMENT_LOAD_OP_DONT_CARE;

			// VK_KHR_load_store_op_none
			case RenderPass::Attachment::Operation::LOAD_NONE: return VK_ATTACHMENT_LOAD_OP_NONE_KHR;
			default: return VkAttachmentLoadOp(0);
		}
	}

	constexpr RenderPass::Attachment::Operation::Load AttachmentLoadOperationToRaindrop(const VkAttachmentLoadOp& op){
		switch (op){
			case VK_ATTACHMENT_LOAD_OP_LOAD: return RenderPass::Attachment::Operation::LOAD;
			case VK_ATTACHMENT_LOAD_OP_CLEAR: return RenderPass::Attachment::Operation::CLEAR;
			case VK_ATTACHMENT_LOAD_OP_DONT_CARE: return RenderPass::Attachment::Operation::DONT_LOAD;
			case VK_ATTACHMENT_LOAD_OP_NONE_KHR: return RenderPass::Attachment::Operation::LOAD_NONE;
			default: return RenderPass::Attachment::Operation::Load(0);
		}
	}

	constexpr VkAttachmentStoreOp AttachmentStoreOperationToVulkan(const RenderPass::Attachment::Operation::Store& op){
		switch (op){
			case RenderPass::Attachment::Operation::STORE: return VK_ATTACHMENT_STORE_OP_STORE;
			case RenderPass::Attachment::Operation::DONT_STORE: return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			case RenderPass::Attachment::Operation::STORE_NONE: return VK_ATTACHMENT_STORE_OP_NONE;
			default: return VkAttachmentStoreOp(0);
		}
	}

	constexpr RenderPass::Attachment::Operation::Store AttachmentLoadOperationToRaindrop(const VkAttachmentStoreOp& op){
		switch (op){
			case VK_ATTACHMENT_STORE_OP_STORE: return RenderPass::Attachment::Operation::STORE;
			case VK_ATTACHMENT_STORE_OP_DONT_CARE: return RenderPass::Attachment::Operation::DONT_STORE;
			case VK_ATTACHMENT_STORE_OP_NONE: return RenderPass::Attachment::Operation::STORE_NONE;
			default: return RenderPass::Attachment::Operation::Store(0);
		}
	}

	constexpr VkSubpassDescriptionFlags SubpassFlagsToVulkan(const RenderPass::Subpass::Flags& flags){
		VkSubpassDescriptionFlags out = 0;

		// VK_NVX_multiview_per_view_attributes
		if (flags.has(RenderPass::Subpass::Flags::PER_VIEW_ATTRIBUTES)) out |= VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX;
		if (flags.has(RenderPass::Subpass::Flags::PER_VIEW_POSITION_X_ONLY)) out |= VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX;

		// VK_QCOM_render_pass_shader_resolve
		if (flags.has(RenderPass::Subpass::Flags::FRAGMENT_REGION)) out |= VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM;
		if (flags.has(RenderPass::Subpass::Flags::SHADER_RESOLVE)) out |= VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM;

		// VK_EXT_rasterization_order_attachment_access
		if (flags.has(RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS)) out |= VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_ARM;
		if (flags.has(RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS)) out |= VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM;
		if (flags.has(RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS)) out |= VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM;

		// VK_EXT_legacy_dithering
		if (flags.has(RenderPass::Subpass::Flags::ENABLE_LEGACY_DITHERING)) out |= VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT;

		return out;
	}

	constexpr RenderPass::Subpass::Flags SubpassFlagsToRaindrop(const VkSubpassDescriptionFlags& flags){
		RenderPass::Subpass::Flags out = 0;

		if (flags & VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX) out |= RenderPass::Subpass::Flags::PER_VIEW_ATTRIBUTES;
		if (flags & VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX) out |= RenderPass::Subpass::Flags::PER_VIEW_POSITION_X_ONLY;
		if (flags & VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM) out |= RenderPass::Subpass::Flags::FRAGMENT_REGION;
		if (flags & VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM) out |= RenderPass::Subpass::Flags::SHADER_RESOLVE;
		if (flags & VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_ARM) out |= RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS;
		if (flags & VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM) out |= RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS;
		if (flags & VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM) out |= RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS;
		if (flags & VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT) out |= RenderPass::Subpass::Flags::ENABLE_LEGACY_DITHERING;

		return out;
	}

	constexpr VkSubpassDescriptionFlagBits SubpassFlagsBitsToVulkan(const RenderPass::Subpass::Flags::Bits& bit){
		switch (bit){
			case RenderPass::Subpass::Flags::PER_VIEW_ATTRIBUTES: return VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX;
			case RenderPass::Subpass::Flags::PER_VIEW_POSITION_X_ONLY: return VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX;
			case RenderPass::Subpass::Flags::FRAGMENT_REGION: return VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM;
			case RenderPass::Subpass::Flags::SHADER_RESOLVE: return VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM;
			case RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS: return VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_ARM;
			case RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS: return VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM;
			case RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS: return VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM;
			case RenderPass::Subpass::Flags::ENABLE_LEGACY_DITHERING: return VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT;
			default: return VkSubpassDescriptionFlagBits(0);
		}
	}

	constexpr RenderPass::Subpass::Flags::Bits SubpassFlagsBitsToRaindrop(const VkSubpassDescriptionFlagBits& bit){
		switch (bit){
			case VK_SUBPASS_DESCRIPTION_PER_VIEW_ATTRIBUTES_BIT_NVX: return RenderPass::Subpass::Flags::PER_VIEW_ATTRIBUTES;
			case VK_SUBPASS_DESCRIPTION_PER_VIEW_POSITION_X_ONLY_BIT_NVX: return RenderPass::Subpass::Flags::PER_VIEW_POSITION_X_ONLY;
			case VK_SUBPASS_DESCRIPTION_FRAGMENT_REGION_BIT_QCOM: return RenderPass::Subpass::Flags::FRAGMENT_REGION;
			case VK_SUBPASS_DESCRIPTION_SHADER_RESOLVE_BIT_QCOM: return RenderPass::Subpass::Flags::SHADER_RESOLVE;
			case VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS_BIT_ARM: return RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS;
			case VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_ARM: return RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS;
			case VK_SUBPASS_DESCRIPTION_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_ARM: return RenderPass::Subpass::Flags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS;
			case VK_SUBPASS_DESCRIPTION_ENABLE_LEGACY_DITHERING_BIT_EXT: return RenderPass::Subpass::Flags::ENABLE_LEGACY_DITHERING;
			default: return RenderPass::Subpass::Flags::NONE;
		}
	}

	constexpr VkDependencyFlags SubpassDependencyFlagsToVulkan(const RenderPass::Dependency::Flags& flags){
		VkDependencyFlags out = 0;

		if (flags.has(RenderPass::Dependency::Flags::BY_REGION)) out |= VK_DEPENDENCY_BY_REGION_BIT;
		if (flags.has(RenderPass::Dependency::Flags::DEVICE_GROUP)) out |= VK_DEPENDENCY_DEVICE_GROUP_BIT;
		if (flags.has(RenderPass::Dependency::Flags::VIEW_LOCAL)) out |= VK_DEPENDENCY_VIEW_LOCAL_BIT;

		// VK_EXT_attachment_feedback_loop_layout
		if (flags.has(RenderPass::Dependency::Flags::FEEDBACK_LOOP)) out |= VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT;

		return out;
	}

	constexpr RenderPass::Dependency::Flags SubpassDependencyFlagsToRaindrop(const VkDependencyFlags& flags){
		RenderPass::Dependency::Flags out = RenderPass::Dependency::Flags::NONE;

		if (flags & VK_DEPENDENCY_BY_REGION_BIT) out |= RenderPass::Dependency::Flags::BY_REGION;
		if (flags & VK_DEPENDENCY_DEVICE_GROUP_BIT) out |= RenderPass::Dependency::Flags::DEVICE_GROUP;
		if (flags & VK_DEPENDENCY_VIEW_LOCAL_BIT) out |= RenderPass::Dependency::Flags::VIEW_LOCAL;
		if (flags & VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT) out |= RenderPass::Dependency::Flags::FEEDBACK_LOOP;

		return out;
	}

	constexpr VkDependencyFlagBits SubpassDependencyFlagsBitsToVulkan(const RenderPass::Dependency::Flags::Bits& bit){
		switch (bit){
			case RenderPass::Dependency::Flags::BY_REGION: return VK_DEPENDENCY_BY_REGION_BIT;
			case RenderPass::Dependency::Flags::DEVICE_GROUP: return VK_DEPENDENCY_DEVICE_GROUP_BIT;
			case RenderPass::Dependency::Flags::VIEW_LOCAL: return VK_DEPENDENCY_VIEW_LOCAL_BIT;
			case RenderPass::Dependency::Flags::FEEDBACK_LOOP: return VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT;
			default: return VkDependencyFlagBits(0);
		}
	}

	constexpr RenderPass::Dependency::Flags::Bits SubpassDependencyFlagsBitsToRaindrop(const VkDependencyFlagBits& bit){
		switch (bit){
			case VK_DEPENDENCY_BY_REGION_BIT: return RenderPass::Dependency::Flags::BY_REGION;
			case VK_DEPENDENCY_DEVICE_GROUP_BIT: return RenderPass::Dependency::Flags::DEVICE_GROUP;
			case VK_DEPENDENCY_VIEW_LOCAL_BIT: return RenderPass::Dependency::Flags::VIEW_LOCAL;
			case VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT: return RenderPass::Dependency::Flags::FEEDBACK_LOOP;
			default: return RenderPass::Dependency::Flags::NONE;
		}
	}
}

#endif