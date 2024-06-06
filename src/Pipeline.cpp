#include <Raindrop/Pipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/Renderer.hpp>
#include <Raindrop_internal/Graphics/Context.hpp>
#include <Raindrop_internal/Context.hpp>
#include <Raindrop/Exceptions/VulkanExceptions.hpp>
#include <Raindrop/RenderPass.hpp>
#include <Raindrop_internal/RenderPass.hpp>
#include <Raindrop_internal/Vertex.hpp>
#include <Raindrop/CommandBuffer.hpp>
#include <Raindrop_internal/CommandBuffer.hpp>
#include <Raindrop_internal/Operators.hpp>

#include <fstream>

#define LOGGER _impl->context->getInternalContext()->getLogger()
#define LAYOUT_INFO _impl->info
#define SHADER_INFO _impl->info
#define INFO _impl->info.internal
#define GRAPHICS_CONTEXT _impl->context->getInternalContext()->getRenderer().getContext()
#define VIEWPORT_INFO reinterpret_cast<VkViewport*>(_data)
#define SCISSOR_INFO reinterpret_cast<VkRect2D*>(_data)
#define ATTACHMENT_INFO reinterpret_cast<VkPipelineColorBlendAttachmentState*>(_data)
#define STENCIL_OP_STATE_INFO reinterpret_cast<VkStencilOpState*>(_data)

namespace Raindrop{

	VkShaderStageFlags toVulkan(const Pipeline::Shader::Stage& s){
		VkShaderStageFlags out = 0;
		if (s.has(Pipeline::Shader::Stage::VERTEX)) out |= VK_SHADER_STAGE_VERTEX_BIT;
		if (s.has(Pipeline::Shader::Stage::TESSELLATION_CONTROL)) out |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (s.has(Pipeline::Shader::Stage::TESSELLATION_EVALUATION)) out |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if (s.has(Pipeline::Shader::Stage::GEOMETRY)) out |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if (s.has(Pipeline::Shader::Stage::FRAGMENT)) out |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (s.has(Pipeline::Shader::Stage::COMPUTE)) out |= VK_SHADER_STAGE_COMPUTE_BIT;
			
			// VK_KHR_ray_tracing_pipeline
		if (s.has(Pipeline::Shader::Stage::RAYGEN)) out |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		if (s.has(Pipeline::Shader::Stage::ANY_HIT)) out |= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
		if (s.has(Pipeline::Shader::Stage::CLOSEST_HIT)) out |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		if (s.has(Pipeline::Shader::Stage::MISS)) out |= VK_SHADER_STAGE_MISS_BIT_KHR;
		if (s.has(Pipeline::Shader::Stage::INTERSECTION)) out |= VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
		if (s.has(Pipeline::Shader::Stage::CALLABLE)) out |= VK_SHADER_STAGE_CALLABLE_BIT_KHR;

			// VK_EXT_mesh_shader
		if (s.has(Pipeline::Shader::Stage::TASK)) out |= VK_SHADER_STAGE_TASK_BIT_EXT;
		if (s.has(Pipeline::Shader::Stage::MESH)) out |= VK_SHADER_STAGE_MESH_BIT_EXT;
			
			// VK_HUAWEI_subpass_shading
		if (s.has(Pipeline::Shader::Stage::SUBPASS_SHADING)) out |= VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI;

			// VK_HUAWEI_cluster_culling_shader
		if (s.has(Pipeline::Shader::Stage::CLUSTER_CULLING)) out |= VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI;
		
		return out;
	}

	VkShaderStageFlagBits toVulkan(const Pipeline::Shader::Stage::Bits& s){
		switch (s){
			case Pipeline::Shader::Stage::VERTEX: return VK_SHADER_STAGE_VERTEX_BIT;
			case Pipeline::Shader::Stage::TESSELLATION_CONTROL: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case Pipeline::Shader::Stage::TESSELLATION_EVALUATION: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case Pipeline::Shader::Stage::GEOMETRY: return VK_SHADER_STAGE_GEOMETRY_BIT;
			case Pipeline::Shader::Stage::FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case Pipeline::Shader::Stage::COMPUTE: return VK_SHADER_STAGE_COMPUTE_BIT;
				
				// VK_KHR_ray_tracing_pipeline
			case Pipeline::Shader::Stage::RAYGEN: return VK_SHADER_STAGE_RAYGEN_BIT_KHR;
			case Pipeline::Shader::Stage::ANY_HIT: return VK_SHADER_STAGE_ANY_HIT_BIT_KHR;
			case Pipeline::Shader::Stage::CLOSEST_HIT: return VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
			case Pipeline::Shader::Stage::MISS: return VK_SHADER_STAGE_MISS_BIT_KHR;
			case Pipeline::Shader::Stage::INTERSECTION: return VK_SHADER_STAGE_INTERSECTION_BIT_KHR;
			case Pipeline::Shader::Stage::CALLABLE: return VK_SHADER_STAGE_CALLABLE_BIT_KHR;

				// VK_EXT_mesh_shader
			case Pipeline::Shader::Stage::TASK: return VK_SHADER_STAGE_TASK_BIT_EXT;
			case Pipeline::Shader::Stage::MESH: return VK_SHADER_STAGE_MESH_BIT_EXT;
				
				// VK_HUAWEI_subpass_shading
			case Pipeline::Shader::Stage::SUBPASS_SHADING: return VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI;

				// VK_HUAWEI_cluster_culling_shader
			case Pipeline::Shader::Stage::CLUSTER_CULLING: return VK_SHADER_STAGE_CLUSTER_CULLING_BIT_HUAWEI;
		}

		throw std::runtime_error("Invalid stage flag");
	}

	VkPipelineShaderStageCreateFlags toVulkan(const Pipeline::Shader::Flags& f){
		VkPipelineShaderStageCreateFlags out = 0;

		if (f.has(Pipeline::Shader::Flags::ALLOW_VARYING_SUBGROUP_SIZE)) out |= VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT;
		if (f.has(Pipeline::Shader::Flags::REQUIRE_FULL_SUBGROUPS)) out |= VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT;

		return out;
	}

	VkDynamicState toVulkan(const Pipeline::DynamicState& s){
		switch (s){
			case Pipeline::DynamicState::VIEWPORT: return VK_DYNAMIC_STATE_VIEWPORT;
			case Pipeline::DynamicState::SCISSOR: return VK_DYNAMIC_STATE_SCISSOR;
			case Pipeline::DynamicState::LINE_WIDTH: return VK_DYNAMIC_STATE_LINE_WIDTH;
			case Pipeline::DynamicState::DEPTH_BIAS: return VK_DYNAMIC_STATE_DEPTH_BIAS;
			case Pipeline::DynamicState::BLEND_CONSTANTS: return VK_DYNAMIC_STATE_BLEND_CONSTANTS;
			case Pipeline::DynamicState::DEPTH_BOUNDS: return VK_DYNAMIC_STATE_DEPTH_BOUNDS;
			case Pipeline::DynamicState::STENCIL_COMPARE_MASK: return VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK;
			case Pipeline::DynamicState::STENCIL_WRITE_MASK: return VK_DYNAMIC_STATE_STENCIL_WRITE_MASK;
			case Pipeline::DynamicState::STENCIL_REFERENCE: return VK_DYNAMIC_STATE_STENCIL_REFERENCE;
			case Pipeline::DynamicState::CULL_MODE: return VK_DYNAMIC_STATE_CULL_MODE;
			case Pipeline::DynamicState::FRONT_FACE: return VK_DYNAMIC_STATE_FRONT_FACE;
			case Pipeline::DynamicState::PRIMITIVE_TOPOLOGY: return VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY;
			case Pipeline::DynamicState::VIEWPORT_WITH_COUNT: return VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT;
			case Pipeline::DynamicState::SCISSOR_WITH_COUNT: return VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT;
			case Pipeline::DynamicState::VERTEX_INPUT_BINDING_STRIDE: return VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE;
			case Pipeline::DynamicState::DEPTH_TEST_ENABLE: return VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE;
			case Pipeline::DynamicState::DEPTH_WRITE_ENABLE: return VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE;
			case Pipeline::DynamicState::DEPTH_COMPARE_OP: return VK_DYNAMIC_STATE_DEPTH_COMPARE_OP;
			case Pipeline::DynamicState::DEPTH_BOUNDS_TEST_ENABLE: return VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE;
			case Pipeline::DynamicState::STENCIL_TEST_ENABLE: return VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE;
			case Pipeline::DynamicState::STENCIL_OP: return VK_DYNAMIC_STATE_STENCIL_OP;
			case Pipeline::DynamicState::RASTERIZER_DISCARD_ENABLE: return VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE;
			case Pipeline::DynamicState::DEPTH_BIAS_ENABLE: return VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE;
			case Pipeline::DynamicState::PRIMITIVE_RESTART_ENABLE: return VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE;

			// VK_NV_clip_space_w_scaling
			case Pipeline::DynamicState::VIEWPORT_W_SCALING: return VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV;

			// VK_EXT_discard_rectangles
			case Pipeline::DynamicState::DISCARD_RECTANGLE: return VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT;
			case Pipeline::DynamicState::DISCARD_RECTANGLE_ENABLE: return VK_DYNAMIC_STATE_DISCARD_RECTANGLE_ENABLE_EXT;
			case Pipeline::DynamicState::DISCARD_RECTANGLE_MODE: return VK_DYNAMIC_STATE_DISCARD_RECTANGLE_MODE_EXT;

			// VK_EXT_sample_locations
			case Pipeline::DynamicState::SAMPLE_LOCATIONS: return VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT;

			// VK_KHR_ray_tracing_pipeline
			case Pipeline::DynamicState::RAY_TRACING_PIPELINE_STACK_SIZE: return VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR;

			// VK_NV_shading_rate_image
			case Pipeline::DynamicState::VIEWPORT_SHADING_RATE_PALETTE: return VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV;
			case Pipeline::DynamicState::VIEWPORT_COARSE_SAMPLE_ORDER: return VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV;

			// VK_NV_scissor_exclusive
			case Pipeline::DynamicState::EXCLUSIVE_SCISSOR_ENABLE: return VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_ENABLE_NV;
			case Pipeline::DynamicState::EXCLUSIVE_SCISSOR: return VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV;

			// VK_KHR_fragment_shading_rate
			case Pipeline::DynamicState::FRAGMENT_SHADING_RATE: return VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR;

			// VK_EXT_vertex_input_dynamic_state
			case Pipeline::DynamicState::VERTEX_INPUT: return VK_DYNAMIC_STATE_VERTEX_INPUT_EXT;

			// VK_EXT_extended_dynamic_state2
			case Pipeline::DynamicState::PATCH_CONTROL_POINTS: return VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT;
			case Pipeline::DynamicState::LOGIC_OP: return VK_DYNAMIC_STATE_LOGIC_OP_EXT;

			// VK_EXT_color_write_enable
			case Pipeline::DynamicState::COLOR_WRITE_ENABLE: return VK_DYNAMIC_STATE_COLOR_WRITE_ENABLE_EXT;

			// VK_EXT_extended_dynamic_state3
			case Pipeline::DynamicState::DEPTH_CLAMP_ENABLE: return VK_DYNAMIC_STATE_DEPTH_CLAMP_ENABLE_EXT;
			case Pipeline::DynamicState::POLYGON_MODE: return VK_DYNAMIC_STATE_POLYGON_MODE_EXT;
			case Pipeline::DynamicState::RASTERIZATION_SAMPLES: return VK_DYNAMIC_STATE_RASTERIZATION_SAMPLES_EXT;
			case Pipeline::DynamicState::SAMPLE_MASK: return VK_DYNAMIC_STATE_SAMPLE_MASK_EXT;
			case Pipeline::DynamicState::ALPHA_TO_COVERAGE_ENABLE: return VK_DYNAMIC_STATE_ALPHA_TO_COVERAGE_ENABLE_EXT;
			case Pipeline::DynamicState::ALPHA_TO_ONE_ENABLE: return VK_DYNAMIC_STATE_ALPHA_TO_ONE_ENABLE_EXT;
			case Pipeline::DynamicState::LOGIC_OP_ENABLE: return VK_DYNAMIC_STATE_LOGIC_OP_ENABLE_EXT;
			case Pipeline::DynamicState::COLOR_BLEND_ENABLE: return VK_DYNAMIC_STATE_COLOR_BLEND_ENABLE_EXT;
			case Pipeline::DynamicState::COLOR_BLEND_EQUATION: return VK_DYNAMIC_STATE_COLOR_BLEND_EQUATION_EXT;
			case Pipeline::DynamicState::COLOR_WRITE_MASK: return VK_DYNAMIC_STATE_COLOR_WRITE_MASK_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_KHR_maintenance2
			case Pipeline::DynamicState::TESSELLATION_DOMAIN_ORIGIN: return VK_DYNAMIC_STATE_TESSELLATION_DOMAIN_ORIGIN_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_transform_feedback
			case Pipeline::DynamicState::RASTERIZATION_STREAM: return VK_DYNAMIC_STATE_RASTERIZATION_STREAM_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_conservative_rasterization
			case Pipeline::DynamicState::CONSERVATIVE_RASTERIZATION_MODE: return VK_DYNAMIC_STATE_CONSERVATIVE_RASTERIZATION_MODE_EXT;
			case Pipeline::DynamicState::EXTRA_PRIMITIVE_OVERESTIMATION_SIZE: return VK_DYNAMIC_STATE_EXTRA_PRIMITIVE_OVERESTIMATION_SIZE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_depth_clip_enable
			case Pipeline::DynamicState::DEPTH_CLIP_ENABLE: return VK_DYNAMIC_STATE_DEPTH_CLIP_ENABLE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_sample_locations
			case Pipeline::DynamicState::SAMPLE_LOCATIONS_ENABLE: return VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_ENABLE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_blend_operation_advanced
			case Pipeline::DynamicState::COLOR_BLEND_ADVANCED: return VK_DYNAMIC_STATE_COLOR_BLEND_ADVANCED_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_provoking_vertex
			case Pipeline::DynamicState::PROVOKING_VERTEX_MODE: return VK_DYNAMIC_STATE_PROVOKING_VERTEX_MODE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_line_rasterization
			case Pipeline::DynamicState::LINE_RASTERIZATION_MODE: return VK_DYNAMIC_STATE_LINE_RASTERIZATION_MODE_EXT;
			case Pipeline::DynamicState::LINE_STIPPLE_ENABLE: return VK_DYNAMIC_STATE_LINE_STIPPLE_ENABLE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_EXT_depth_clip_control
			case Pipeline::DynamicState::DEPTH_CLIP_NEGATIVE_ONE_TO_ONE: return VK_DYNAMIC_STATE_DEPTH_CLIP_NEGATIVE_ONE_TO_ONE_EXT;

			// VK_EXT_extended_dynamic_state3 & VK_NV_clip_space_w_scaling
			case Pipeline::DynamicState::VIEWPORT_W_SCALING_ENABLE: return VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_ENABLE_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_viewport_swizzle
			case Pipeline::DynamicState::VIEWPORT_SWIZZLE: return VK_DYNAMIC_STATE_VIEWPORT_SWIZZLE_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_fragment_coverage_to_color
			case Pipeline::DynamicState::COVERAGE_TO_COLOR_ENABLE: return VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_ENABLE_NV;
			case Pipeline::DynamicState::COVERAGE_TO_COLOR_LOCATION: return VK_DYNAMIC_STATE_COVERAGE_TO_COLOR_LOCATION_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_framebuffer_mixed_samples
			case Pipeline::DynamicState::COVERAGE_MODULATION_MODE: return VK_DYNAMIC_STATE_COVERAGE_MODULATION_MODE_NV;
			case Pipeline::DynamicState::COVERAGE_MODULATION_TABLE_ENABLE: return VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_ENABLE_NV;
			case Pipeline::DynamicState::COVERAGE_MODULATION_TABLE: return VK_DYNAMIC_STATE_COVERAGE_MODULATION_TABLE_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_shading_rate_image
			case Pipeline::DynamicState::SHADING_RATE_IMAGE_ENABLE: return VK_DYNAMIC_STATE_SHADING_RATE_IMAGE_ENABLE_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_representative_fragment_test
			case Pipeline::DynamicState::REPRESENTATIVE_FRAGMENT_TEST_ENABLE: return VK_DYNAMIC_STATE_REPRESENTATIVE_FRAGMENT_TEST_ENABLE_NV;

			// VK_EXT_extended_dynamic_state3 & VK_NV_coverage_reduction_mode
			case Pipeline::DynamicState::COVERAGE_REDUCTION_MODE: return VK_DYNAMIC_STATE_COVERAGE_REDUCTION_MODE_NV;

			// VK_EXT_attachment_feedback_loop_dynamic_state
			case Pipeline::DynamicState::ATTACHMENT_FEEDBACK_LOOP_ENABLE: return VK_DYNAMIC_STATE_ATTACHMENT_FEEDBACK_LOOP_ENABLE_EXT;

			// VK_KHR_line_rasterization
			case Pipeline::DynamicState::LINE_STIPPLE: return VK_DYNAMIC_STATE_LINE_STIPPLE_KHR;
		}
		throw std::runtime_error("Invalid dynamic state");
	}

	VkStencilOp toVulkan(const Pipeline::StencilOperation& op){
		switch (op){
			case Pipeline::StencilOperation::KEEP: return VK_STENCIL_OP_KEEP;
			case Pipeline::StencilOperation::ZERO: return VK_STENCIL_OP_ZERO;
			case Pipeline::StencilOperation::REPLACE: return VK_STENCIL_OP_REPLACE;
			case Pipeline::StencilOperation::INCREMENT_AND_CLAMP: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case Pipeline::StencilOperation::DECREMENT_AND_CLAMP: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case Pipeline::StencilOperation::INVERT: return VK_STENCIL_OP_INVERT;
			case Pipeline::StencilOperation::INCREMENT_AND_WRAP: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case Pipeline::StencilOperation::DECREMENT_AND_WRAP: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
		}
		throw std::runtime_error("Invalid stencil operation");
	}

	VkSampleCountFlagBits toVulkan(const Pipeline::SampleCount::Bits& count){
		switch (count){
			case Pipeline::SampleCount::ONE: return VK_SAMPLE_COUNT_1_BIT;
			case Pipeline::SampleCount::TWO: return VK_SAMPLE_COUNT_2_BIT;
			case Pipeline::SampleCount::FOUR: return VK_SAMPLE_COUNT_4_BIT;
			case Pipeline::SampleCount::HEIGHT: return VK_SAMPLE_COUNT_8_BIT;
			case Pipeline::SampleCount::SIXTEEN: return VK_SAMPLE_COUNT_16_BIT;
			case Pipeline::SampleCount::THIRTY_TWO: return VK_SAMPLE_COUNT_32_BIT;
			case Pipeline::SampleCount::SIXTY_FOUR: return VK_SAMPLE_COUNT_64_BIT;
		}
		throw std::runtime_error("Invalid sample count bit");
	}

	VkSampleCountFlags toVulkan(const Pipeline::SampleCount& count){
		VkSampleCountFlags out = 0;
		if (count.has(Pipeline::SampleCount::ONE)) out |= VK_SAMPLE_COUNT_1_BIT;
		if (count.has(Pipeline::SampleCount::TWO)) out |= VK_SAMPLE_COUNT_2_BIT;
		if (count.has(Pipeline::SampleCount::FOUR)) out |= VK_SAMPLE_COUNT_4_BIT;
		if (count.has(Pipeline::SampleCount::HEIGHT)) out |= VK_SAMPLE_COUNT_8_BIT;
		if (count.has(Pipeline::SampleCount::SIXTEEN)) out |= VK_SAMPLE_COUNT_16_BIT;
		if (count.has(Pipeline::SampleCount::THIRTY_TWO)) out |= VK_SAMPLE_COUNT_32_BIT;
		if (count.has(Pipeline::SampleCount::SIXTY_FOUR)) out |= VK_SAMPLE_COUNT_64_BIT;
		return out;
	}

	VkFrontFace toVulkan(const Pipeline::FrontFace& f){
		switch (f){
			case Pipeline::FrontFace::COUNTER_CLOCKWISE: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case Pipeline::FrontFace::CLOCKWISE: return VK_FRONT_FACE_CLOCKWISE;
		}
		throw std::runtime_error("Invalid front face");
	}

	VkPolygonMode toVulkan(const Pipeline::PolygonMode& mode){
		switch (mode){
			case Pipeline::PolygonMode::FILL: return VK_POLYGON_MODE_FILL;
			case Pipeline::PolygonMode::LINE: return VK_POLYGON_MODE_LINE;
			case Pipeline::PolygonMode::POINT: return VK_POLYGON_MODE_POINT;

			// VK_NV_fill_rectangle
			case Pipeline::PolygonMode::FILL_RECTANGLE: return VK_POLYGON_MODE_FILL_RECTANGLE_NV;
		}
		throw std::runtime_error("Invalid polygon mode");
	}

	VkPrimitiveTopology toVulkan(const Pipeline::Topology& t){
		switch (t){
			case Pipeline::Topology::POINT_LIST:					return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case Pipeline::Topology::LINE_LIST:						return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case Pipeline::Topology::LINE_STRIP:					return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case Pipeline::Topology::TRIANGLE_LIST:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case Pipeline::Topology::TRIANGLE_STRIP:				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

			// VK_KHR_portability_subset && VkPhysicalDevicePortabilitySubsetFeaturesKHR::triangleFans
			case Pipeline::Topology::TRIANGLE_FAN:					return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

			case Pipeline::Topology::LINE_LIST_WITH_ADJACENCY:		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
			case Pipeline::Topology::LINE_STRIP_WITH_ADJACENCY:		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
			case Pipeline::Topology::TRIANGLE_LIST_WITH_ADJACENCY:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
			case Pipeline::Topology::TRIANGLE_STRIP_WITH_ADJACENCY:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
			case Pipeline::Topology::PATCH_LIST:					return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
		}
		throw std::runtime_error("Invalid primitive topology");
	}

	VkCullModeFlags toVulkan(const Pipeline::CullMode& mode){
		VkCullModeFlags out = 0;

		if (mode.has(Pipeline::CullMode::FRONT)) out |= VK_CULL_MODE_FRONT_BIT;
		if (mode.has(Pipeline::CullMode::BACK)) out |= VK_CULL_MODE_BACK_BIT;

		return out;
	}

	VkPipelineDepthStencilStateCreateFlags toVulkan(const Pipeline::DepthStencilFlags& flags){
		VkPipelineDepthStencilStateCreateFlags out = 0;

		// VK_EXT_rasterization_order_attachment_access
		if (flags.has(Pipeline::DepthStencilFlags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS)) out |= VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_EXT;
		if (flags.has(Pipeline::DepthStencilFlags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS)) out |= VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_EXT;

		return out;
	}

	VkPipelineColorBlendStateCreateFlags toVulkan(const Pipeline::ColorBlendFlags& flags){
		VkPipelineColorBlendStateCreateFlags out = 0;
		
		// VK_EXT_rasterization_order_attachment_access
		if (flags.has(Pipeline::ColorBlendFlags::RASTERIZATION_ORDER_ATTACHMENT_ACCESS)) out |= VK_PIPELINE_COLOR_BLEND_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_BIT_EXT;

		return out;
	}

	VkPipelineStageFlags toVulkan(const Pipeline::Stage& stage){
		VkPipelineStageFlags out = 0;

		if (stage.has(Pipeline::Stage::TOP_OF_PIPE)) 						out |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		if (stage.has(Pipeline::Stage::DRAW_INDIRECT)) 						out |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
		if (stage.has(Pipeline::Stage::VERTEX_INPUT)) 						out |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
		if (stage.has(Pipeline::Stage::VERTEX_SHADER)) 						out |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		if (stage.has(Pipeline::Stage::TESSELLATION_CONTROL_SHADER)) 		out |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
		if (stage.has(Pipeline::Stage::TESSELLATION_EVALUATION_SHADER)) 	out |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
		if (stage.has(Pipeline::Stage::GEOMETRY_SHADER)) 					out |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
		if (stage.has(Pipeline::Stage::FRAGMENT_SHADER)) 					out |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		if (stage.has(Pipeline::Stage::EARLY_FRAGMENT_TESTS)) 				out |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		if (stage.has(Pipeline::Stage::LATE_FRAGMENT_TESTS)) 				out |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		if (stage.has(Pipeline::Stage::COLOR_ATTACHMENT_OUTPUT)) 			out |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		if (stage.has(Pipeline::Stage::COMPUTE_SHADER)) 					out |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
		if (stage.has(Pipeline::Stage::TRANSFER)) 							out |= VK_PIPELINE_STAGE_TRANSFER_BIT;
		if (stage.has(Pipeline::Stage::BOTTOM_OF_PIPE)) 					out |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		if (stage.has(Pipeline::Stage::HOST)) 								out |= VK_PIPELINE_STAGE_HOST_BIT;
		if (stage.has(Pipeline::Stage::ALL_GRAPHICS)) 						out |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		if (stage.has(Pipeline::Stage::ALL_COMMANDS)) 						out |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
		if (stage.has(Pipeline::Stage::NONE)) 								out |= VK_PIPELINE_STAGE_NONE;

		// VK_EXT_transform_feedback
		if (stage.has(Pipeline::Stage::TRANSFORM_FEEDBACK)) 				out |= VK_PIPELINE_STAGE_TRANSFORM_FEEDBACK_BIT_EXT;

		// VK_EXT_conditional_rendering
		if (stage.has(Pipeline::Stage::CONDITIONAL_RENDERING)) 				out |= VK_PIPELINE_STAGE_CONDITIONAL_RENDERING_BIT_EXT;

		// VK_KHR_acceleration_structure
		if (stage.has(Pipeline::Stage::ACCELERATION_STRUCTURE_BUILD)) 		out |= VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR;

		// VK_KHR_ray_tracing_pipeline
		if (stage.has(Pipeline::Stage::RAY_TRACING_SHADER)) 				out |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR;

		// VK_EXT_fragment_density_map
		if (stage.has(Pipeline::Stage::FRAGMENT_DENSITY_PROCESS)) 			out |= VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT;

		// VK_KHR_fragment_shading_rate
		if (stage.has(Pipeline::Stage::FRAGMENT_SHADING_RATE_ATTACHMENT)) 	out |= VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;

		// VK_NV_device_generated_commands
		if (stage.has(Pipeline::Stage::COMMAND_PREPROCES)) 					out |= VK_PIPELINE_STAGE_COMMAND_PREPROCESS_BIT_NV;

		// VK_EXT_mesh_shader
		if (stage.has(Pipeline::Stage::TASK_SHADER)) 						out |= VK_PIPELINE_STAGE_TASK_SHADER_BIT_EXT;

		// VK_EXT_mesh_shader
		if (stage.has(Pipeline::Stage::MESH_SHADER)) 						out |= VK_PIPELINE_STAGE_MESH_SHADER_BIT_EXT;

		return out;
	}

	VkPipelineCreateFlags toVulkan(const Pipeline::Flags& flags){
		VkPipelineCreateFlags out = 0;

		if (flags.has(Pipeline::Flags::DISABLE_OPTIMIZATION))							out |= VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT;
		if (flags.has(Pipeline::Flags::ALLOW_DERIVATIVES))								out |= VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT;
		if (flags.has(Pipeline::Flags::DERIVATIVE))										out |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
		if (flags.has(Pipeline::Flags::VIEW_INDEX_FROM_DEVICE_INDEX))					out |= VK_PIPELINE_CREATE_VIEW_INDEX_FROM_DEVICE_INDEX_BIT;
		if (flags.has(Pipeline::Flags::DISPATCH_BASE))									out |= VK_PIPELINE_CREATE_DISPATCH_BASE_BIT;
		if (flags.has(Pipeline::Flags::FAIL_ON_PIPELINE_COMPILE_REQUIRED))				out |= VK_PIPELINE_CREATE_FAIL_ON_PIPELINE_COMPILE_REQUIRED_BIT;
		if (flags.has(Pipeline::Flags::EARLY_RETURN_ON_FAILURE))						out |= VK_PIPELINE_CREATE_EARLY_RETURN_ON_FAILURE_BIT;

		// VK_KHR_dynamic_rendering with VK_KHR_fragment_shading_rate
		if (flags.has(Pipeline::Flags::RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT))		out |= VK_PIPELINE_CREATE_RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;

		// VK_KHR_dynamic_rendering with VK_EXT_fragment_density_map
		if (flags.has(Pipeline::Flags::RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT))		out |= VK_PIPELINE_CREATE_RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT_BIT_EXT;

		// VK_KHR_ray_tracing_pipeline
		if (flags.has(Pipeline::Flags::RAY_TRACING_NO_NULL_ANY_HIT_SHADERS))			out |= VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_ANY_HIT_SHADERS_BIT_KHR; 
		if (flags.has(Pipeline::Flags::RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS))		out |= VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS_BIT_KHR;
		if (flags.has(Pipeline::Flags::RAY_TRACING_NO_NULL_MISS_SHADERS))				out |= VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_MISS_SHADERS_BIT_KHR;
		if (flags.has(Pipeline::Flags::RAY_TRACING_NO_NULL_INTERSECTION_SHADERS))		out |= VK_PIPELINE_CREATE_RAY_TRACING_NO_NULL_INTERSECTION_SHADERS_BIT_KHR;
		if (flags.has(Pipeline::Flags::RAY_TRACING_SKIP_TRIANGLES))						out |= VK_PIPELINE_CREATE_RAY_TRACING_SKIP_TRIANGLES_BIT_KHR;
		if (flags.has(Pipeline::Flags::RAY_TRACING_SKIP_AABBS))							out |= VK_PIPELINE_CREATE_RAY_TRACING_SKIP_AABBS_BIT_KHR;
		if (flags.has(Pipeline::Flags::RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY))	out |= VK_PIPELINE_CREATE_RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY_BIT_KHR;
		
		// VK_NV_ray_tracing
		if (flags.has(Pipeline::Flags::DEFER_COMPILE))						 			out |= VK_PIPELINE_CREATE_DEFER_COMPILE_BIT_NV; 
		
		// VK_KHR_pipeline_executable_properties
		if (flags.has(Pipeline::Flags::CAPTURE_STATISTICS))						 		out |= VK_PIPELINE_CREATE_CAPTURE_STATISTICS_BIT_KHR;

		// VK_KHR_pipeline_executable_properties
		if (flags.has(Pipeline::Flags::CAPTURE_INTERNAL_REPRESENTATIONS))				out |= VK_PIPELINE_CREATE_CAPTURE_INTERNAL_REPRESENTATIONS_BIT_KHR;

		// VK_NV_device_generated_commands
		if (flags.has(Pipeline::Flags::INDIRECT_BINDABLE))						 		out |= VK_PIPELINE_CREATE_INDIRECT_BINDABLE_BIT_NV;

		// VK_KHR_pipeline_library
		if (flags.has(Pipeline::Flags::LIBRARY))										out |= VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;

		// VK_EXT_descriptor_buffer
		if (flags.has(Pipeline::Flags::DESCRIPTOR_BUFFER))						 		out |= VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;

		// VK_EXT_graphics_pipeline_library
		if (flags.has(Pipeline::Flags::RETAIN_LINK_TIME_OPTIMIZATION_INFO))				out |= VK_PIPELINE_CREATE_RETAIN_LINK_TIME_OPTIMIZATION_INFO_BIT_EXT;
		if (flags.has(Pipeline::Flags::LINK_TIME_OPTIMIZATION))						 	out |= VK_PIPELINE_CREATE_LINK_TIME_OPTIMIZATION_BIT_EXT;

		// VK_NV_ray_tracing_motion_blur
		if (flags.has(Pipeline::Flags::RAY_TRACING_ALLOW_MOTION))						out |= VK_PIPELINE_CREATE_RAY_TRACING_ALLOW_MOTION_BIT_NV;

		// VK_EXT_attachment_feedback_loop_layout
		if (flags.has(Pipeline::Flags::COLOR_ATTACHMENT_FEEDBACK_LOOP))					out |= VK_PIPELINE_CREATE_COLOR_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;
		if (flags.has(Pipeline::Flags::DEPTH_STENCIL_ATTACHMENT_FEEDBACK_LOOP))			out |= VK_PIPELINE_CREATE_DEPTH_STENCIL_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT;

		// VK_EXT_opacity_micromap
		if (flags.has(Pipeline::Flags::RAY_TRACING_OPACITY_MICROMAP))					out |= VK_PIPELINE_CREATE_RAY_TRACING_OPACITY_MICROMAP_BIT_EXT;

		// VK_EXT_pipeline_protected_access
		if (flags.has(Pipeline::Flags::NO_PROTECTED_ACCESS))							out |= VK_PIPELINE_CREATE_NO_PROTECTED_ACCESS_BIT_EXT;
		if (flags.has(Pipeline::Flags::PROTECTED_ACCESS_ONLY))						 	out |= VK_PIPELINE_CREATE_PROTECTED_ACCESS_ONLY_BIT_EXT;
		
		return out;
	}

	VkStencilOpState toVulkan(const Pipeline::StencilOpState& state){
		return VkStencilOpState{
			.failOp = static_cast<VkStencilOp>(state.getFailOp()),
			.passOp = static_cast<VkStencilOp>(state.getPassOp()),
			.depthFailOp = static_cast<VkStencilOp>(state.getDepthFailOp()),
			.compareOp = static_cast<VkCompareOp>(state.getCompareOp()),
			.compareMask = state.getCompareMask(),
			.writeMask = state.getWriteMask(),
			.reference = state.getReference()
		};
	}

	//--------------------------------------------------------------------
	//-----------------         Pipeline layout          -----------------
	//--------------------------------------------------------------------

	Pipeline::Layout Pipeline::Layout::Create(Context& context){
		return Layout(context);
	}

	Pipeline::Layout::Layout(Context& context){
		_impl = new Impl(context);
	}

	Pipeline::Layout::~Layout(){
		delete _impl;
		_impl = nullptr;
	}

	Pipeline::Layout::Layout(const Layout& other) : _impl{nullptr}{
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Pipeline::Layout& Pipeline::Layout::operator=(const Layout& other){
		*_impl = *other._impl;
		return *this;
	}

	void Pipeline::Layout::initialize(){
		LOGGER->info("Initializing pipeline layout...");
		_impl->layout = std::make_shared<Internal::Graphics::PipelineLayout>(GRAPHICS_CONTEXT, _impl->info);
		LOGGER->info("Pipeline layout initialized with success !");
	}

	void Pipeline::Layout::release(){
		_impl->layout.reset();
	}

	void Pipeline::Layout::setFlags(const Flags& flags){
		LAYOUT_INFO.flags = static_cast<VkPipelineLayoutCreateFlags>(flags.get());
	}

	Pipeline::Layout::Flags Pipeline::Layout::getFlags() const noexcept{
		return static_cast<Flags::Bitset>(LAYOUT_INFO.flags);
	}

	bool Pipeline::Layout::isInitialized() const noexcept{
		return _impl->layout != nullptr;
	}

	void* Pipeline::Layout::getNativeHandle() const{
		return static_cast<void*>(_impl->layout->get());
	}

	Pipeline::Layout::Impl* Pipeline::Layout::getImpl() const noexcept{
		return _impl;
	}

	GUID Pipeline::Layout::getGUID() const noexcept{
		// TODO
	}

	void Pipeline::Layout::setPushConstant(const Stage& stage, const std::size_t size){
		LAYOUT_INFO.pushConstant = VkPushConstantRange{
			.stageFlags = toVulkan(stage),
			.offset = 0,
			.size = static_cast<uint32_t>(size)
		};
	}

	//--------------------------------------------------------------------
	//-----------------             Viewport             -----------------
	//--------------------------------------------------------------------

	Pipeline::Viewport& Pipeline::Viewport::setX(const float& x) noexcept{
		_x = x;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setY(const float& y) noexcept{
		_y = y;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setWidth(const float& width) noexcept{
		_width = width;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setHeight(const float& height) noexcept{
		_height = height;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMinDepth(const float& minDepth) noexcept{
		_minDepth = minDepth;
		return *this;
	}

	Pipeline::Viewport& Pipeline::Viewport::setMaxDepth(const float& maxDepth) noexcept{
		_maxDepth = maxDepth;
		return *this;
	}


	const float& Pipeline::Viewport::getX() const noexcept{
		return _x;
	}

	const float& Pipeline::Viewport::getY() const noexcept{
		return _y;
	}

	const float& Pipeline::Viewport::getWidth() const noexcept{
		return _width;
	}

	const float& Pipeline::Viewport::getHeight() const noexcept{
		return _height;
	}

	const float& Pipeline::Viewport::getMinDepth() const noexcept{
		return _minDepth;
	}

	const float& Pipeline::Viewport::getMaxDepth() const noexcept{
		return _maxDepth;
	}

	//--------------------------------------------------------------------
	//-----------------             Scissor              -----------------
	//--------------------------------------------------------------------

	Pipeline::Scissor& Pipeline::Scissor::setX(const std::int32_t& x) noexcept{
		_x = static_cast<uint32_t>(x);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setY(const std::int32_t& y) noexcept{
		_y = static_cast<uint32_t>(y);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setWidth(const std::uint32_t& width) noexcept{
		_width = static_cast<uint32_t>(width);
		return *this;
	}

	Pipeline::Scissor& Pipeline::Scissor::setHeight(const std::uint32_t& height) noexcept{
		_height = static_cast<uint32_t>(height);
		return *this;
	}

	const std::int32_t& Pipeline::Scissor::getX() const noexcept{
		return _x;
	}

	const std::int32_t& Pipeline::Scissor::getY() const noexcept{
		return _y;
	}

	const std::uint32_t& Pipeline::Scissor::getWidth() const noexcept{
		return _width;
	}

	const std::uint32_t& Pipeline::Scissor::getHeight() const noexcept{
		return _height;
	}

	//--------------------------------------------------------------------
	//-----------------         Color attachments        -----------------
	//--------------------------------------------------------------------


	Pipeline::ColorAttachment::ColorAttachment() noexcept : 
		_blendEnabled{false},
		_srcColorBlendFactor{Color::BlendFactor::SRC_COLOR},
		_dstColorBlendFactor{Color::BlendFactor::ZERO},
		_colorBlendOp{Color::BlendOperation::ADD},
		_srcAlphaBlendFactor{Color::BlendFactor::ONE},
		_dstAlphaBlendFactor{Color::BlendFactor::ZERO},
		_alphaBlendOp{Color::BlendOperation::ADD},
		_writeMask{Color::Components::RGBA}
	{}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::enableBlending(const bool& enable) noexcept{
		_blendEnabled = enable;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept{
		_srcColorBlendFactor = srcColorBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept{
		_dstColorBlendFactor = dstColorBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept{
		_colorBlendOp = colorBlendOp;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept{
		_srcAlphaBlendFactor = srcAlphaBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept{
		_dstAlphaBlendFactor = dstAlphaBlendFactor;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept{
		_alphaBlendOp = alphaBlendOp;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::ColorAttachment::setWriteMask(const Color::Components& writeMask) noexcept{
		_writeMask = writeMask;
		return *this;
	}

	const bool& Pipeline::ColorAttachment::isBlendingEnable() const noexcept{
		return _blendEnabled;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getSrcColorBlendFactor() const noexcept{
		return _srcColorBlendFactor;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getDstColorBlendFactor() const noexcept{
		return _dstColorBlendFactor;
	}

	const Color::BlendOperation& Pipeline::ColorAttachment::getColorBlendOp() const noexcept{
		return _colorBlendOp;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getSrcAlphaBlendFactor() const noexcept{
		return _srcAlphaBlendFactor;
	}

	const Color::BlendFactor& Pipeline::ColorAttachment::getDstAlphaBlendFactor() const noexcept{
		return _dstAlphaBlendFactor;
	}

	const Color::BlendOperation& Pipeline::ColorAttachment::getAlphaBlendOp() const noexcept{
		return _alphaBlendOp;
	}

	const Color::Components& Pipeline::ColorAttachment::getWriteMask() const noexcept{
		return _writeMask;
	}
	
	//--------------------------------------------------------------------
	//-----------------        Stencil Op state          -----------------
	//--------------------------------------------------------------------

	Pipeline::StencilOpState& Pipeline::StencilOpState::setFailOp(const StencilOperation& failOp) noexcept{
		_failOp = failOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setPassOp(const StencilOperation& passOp) noexcept{
		_passOp = passOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setDepthFailOp(const StencilOperation& depthFailOp) noexcept{
		_depthFailOp = depthFailOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareOp(const CompareOperator& compareOp) noexcept{
		_compareOp = compareOp;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setCompareMask(const std::uint32_t& compareMask) noexcept{
		_compareMask = compareMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setWriteMask(const std::uint32_t& writeMask) noexcept{
		_writeMask = writeMask;
		return *this;
	}

	Pipeline::StencilOpState& Pipeline::StencilOpState::setReference(const std::uint32_t& reference) noexcept{
		_reference = reference;
		return *this;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getFailOp() const noexcept{
		return _failOp;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getPassOp() const noexcept{
		return _passOp;
	}

	const Pipeline::StencilOperation& Pipeline::StencilOpState::getDepthFailOp() const noexcept{
		return _depthFailOp;
	}

	const CompareOperator& Pipeline::StencilOpState::getCompareOp() const noexcept{
		return _compareOp;
	}

	const std::uint32_t& Pipeline::StencilOpState::getCompareMask() const noexcept{
		return _compareMask;
	}

	const std::uint32_t& Pipeline::StencilOpState::getWriteMask() const noexcept{
		return _writeMask;
	}

	const std::uint32_t& Pipeline::StencilOpState::getReference() const noexcept{
		return _reference;
	}


	//--------------------------------------------------------------------
	//-----------------        Pipeline shader           -----------------
	//--------------------------------------------------------------------


	Pipeline::Shader Pipeline::Shader::Create(Context& context, const Path& path){
		return Shader(context, path);
	}

	std::shared_ptr<Pipeline::Shader> Pipeline::Shader::Load(Context& context, const Path& path){
		return Asset::Load<Pipeline::Shader>(context, "Shader", path);
	}

	std::vector<char> readFile(const Path& filepath) {
		std::ifstream file{filepath, std::ios::ate | std::ios::binary};

		if (!file.is_open()) {
			spdlog::warn("Failed to open \"{}\" shader file", filepath.string());
			throw std::runtime_error("failed to open file");
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void* Pipeline::Shader::getNativeHandle() const{
		return static_cast<void*>(_impl->module->get());
	}

	Pipeline::Shader::Impl* Pipeline::Shader::getImpl() const noexcept{
		return _impl;
	}

	bool Pipeline::Shader::isInitialized() const noexcept{
		return _impl->module != VK_NULL_HANDLE;
	}

	Pipeline::Shader::Shader(Context& context, const Path& path){
		_impl = new Impl(context);

		std::vector<char> code = readFile(path);
		SHADER_INFO.code = code;
		_impl->module = std::make_shared<Internal::Graphics::Shader>(_impl->context.getInternalContext()->getRenderer().getContext(), SHADER_INFO);
	}
	
	Pipeline::Shader::~Shader(){
		delete _impl;
	}

	//--------------------------------------------------------------------
	//-----------------            Pipeline              -----------------
	//--------------------------------------------------------------------

	Pipeline Pipeline::Create(Context& context){
		return Pipeline(context);
	}

	Pipeline::Pipeline(Context& context){
		_impl = new Impl(context);
	}

	Pipeline::~Pipeline(){
		if (_impl){
			delete _impl;
			_impl = nullptr;
		}
	}

	Pipeline::Pipeline(const Pipeline& other){
		_impl = new Impl(*other._impl->context);
		*_impl = *other._impl;
	}

	Pipeline& Pipeline::operator=(const Pipeline& other){
		*_impl = *other._impl;
		return *this;
	}
	
	void Pipeline::initialize(){
		LOGGER->info("Initializing pipeline...");


		std::vector<VkRect2D> scissors(_impl->info.scissors.size());
		std::vector<VkViewport> viewports(_impl->info.viewports.size());
		std::vector<VkPipelineColorBlendAttachmentState> attachments(_impl->info.attachments.size());
		std::vector<VkVertexInputAttributeDescription> attributes;
		std::vector<VkVertexInputBindingDescription> bindings;
		std::vector<VkDynamicState> dynamicStates(_impl->info.dynamicStates.size());

		
		// copy scissors
		for (std::size_t i=0; i<scissors.size(); i++){
			const auto& dst = _impl->info.scissors[i];
			scissors[i] = VkRect2D{
				.offset = {
					.x = dst.getX(),
					.y = dst.getY()
				},
				.extent = {
					.width = dst.getWidth(),
					.height = dst.getHeight()
				}
			};
		}
		
		// copy viewports
		for (std::size_t i=0; i<viewports.size(); i++){
			const auto& dst = _impl->info.viewports[i];
			viewports[i] = VkViewport{
				.x = dst.getX(),
				.y = dst.getY(),
				.width = dst.getWidth(),
				.height = dst.getHeight(),
				.minDepth = dst.getMinDepth(),
				.maxDepth = dst.getMaxDepth()
			};
		}
		
		// copy attachment descriptions
		for (std::size_t i=0; i<attachments.size(); i++){
			const auto& dst = _impl->info.attachments[i];
			attachments[i] = VkPipelineColorBlendAttachmentState{
				.blendEnable = toVulkan(dst.isBlendingEnable()),
				.srcColorBlendFactor = toVulkan(dst.getSrcColorBlendFactor()),
				.dstColorBlendFactor = toVulkan(dst.getDstColorBlendFactor()),
				.colorBlendOp = toVulkan(dst.getColorBlendOp()),
				.srcAlphaBlendFactor = toVulkan(dst.getSrcAlphaBlendFactor()),
				.dstAlphaBlendFactor = toVulkan(dst.getDstAlphaBlendFactor()),
				.alphaBlendOp = toVulkan(dst.getAlphaBlendOp()),
				.colorWriteMask = toVulkan(dst.getWriteMask())
			};
		}

		uint32_t bindingId = 0;
		for (const auto& binding : _impl->info.bindings){
			bindings.push_back(VkVertexInputBindingDescription{
				.binding = bindingId,
				.stride = static_cast<uint32_t>(binding.first.getStride()),
				.inputRate = toVulkan(binding.second)
			});

			uint32_t location = 0;
			const auto& layout = binding.first;
			for (const auto& attribute : layout.getAttributes()){
				attributes.push_back(VkVertexInputAttributeDescription{
					.location = location,
					.binding = bindingId,
					.format = static_cast<VkFormat>(attribute.format.get()),
					.offset = static_cast<uint32_t>(attribute.offset)
				});

				location++;
			}

			bindingId++;
		}

		for (std::size_t i=0; i<dynamicStates.size(); i++){
			dynamicStates[i] = toVulkan(_impl->info.dynamicStates[i]);
		}

		for (const auto& stage : _impl->info.stages){
			auto shader = stage.shader->getImpl()->module;
			_impl->info.internal.shaders.push_back(shader);

			_impl->info.internal.stages.push_back(VkPipelineShaderStageCreateInfo{
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = toVulkan(stage.flags),
				.stage = toVulkan(stage.stage),
				.module = shader->get(),
				.pName = stage.entryPoint.c_str()
			});
		}

		_impl->info.internal.vertexInfo.pVertexAttributeDescriptions = attributes.data();
		_impl->info.internal.vertexInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());

		_impl->info.internal.vertexInfo.pVertexBindingDescriptions = bindings.data();
		_impl->info.internal.vertexInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());

		_impl->info.internal.colorBlendInfo.pAttachments = attachments.data();
		_impl->info.internal.colorBlendInfo.attachmentCount = static_cast<uint32_t>(attachments.size());

		_impl->info.internal.viewportInfo.viewportCount = static_cast<uint32_t>(viewports.size());
		_impl->info.internal.viewportInfo.pViewports = viewports.data();

		_impl->info.internal.viewportInfo.scissorCount = static_cast<uint32_t>(scissors.size());
		_impl->info.internal.viewportInfo.pScissors = scissors.data();

		_impl->info.internal.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		_impl->info.internal.dynamicStateInfo.pDynamicStates = dynamicStates.data();


		_impl->pipeline = std::make_shared<Internal::Graphics::GraphicsPipeline>(GRAPHICS_CONTEXT, _impl->info.internal);
		LOGGER->info("Pipeline initialized with success !");
	}

	void Pipeline::release(){
		if (_impl){
			*_impl = Impl(*_impl->context);
		}
	}

	bool Pipeline::isInitialized() const noexcept{
		return _impl->pipeline != nullptr;
	}
	
	void* Pipeline::getNativeHandle() const{
		return static_cast<void*>(_impl->pipeline->get());
	}
	
	Pipeline::Impl* Pipeline::getImpl() const noexcept{
		return _impl;
	}
	
	GUID Pipeline::getGUID() const noexcept{

	}

	Pipeline& Pipeline::setFlags(const Flags& flags){
		_impl->info.internal.flags = static_cast<VkPipelineCreateFlags>(flags.get());
		return *this;
	}

	Pipeline& Pipeline::setRenderPass(const RenderPass& renderPass){
		_impl->info.internal.renderPass = renderPass.getImpl()->renderPass;
		return *this;
	}

	Pipeline& Pipeline::setLayout(const Layout& layout){
		_impl->info.internal.pipelineLayout = layout.getImpl()->layout;
		return *this;
	}

	Pipeline::ColorAttachment& Pipeline::addColorAttachment(){
		_impl->info.attachments.push_back({});
		return _impl->info.attachments.back();
	}

	Vertex::Layout& Pipeline::addVertexBinding(const Vertex::InputRate& inputRate){
		_impl->info.bindings.push_back({Vertex::Layout(), inputRate});
		return _impl->info.bindings.back().first;
	}

	Pipeline::Viewport& Pipeline::addViewport(){
		_impl->info.viewports.push_back({});
		return _impl->info.viewports.back();
	}

	Pipeline::Scissor& Pipeline::addScissor(){
		_impl->info.scissors.push_back({});
		return _impl->info.scissors.back();
	}

	Pipeline& Pipeline::setPrimitiveTopology(const Topology& topology){
		INFO.inputAssemblyInfo.topology = toVulkan(topology);
		return *this;
	}

	Pipeline& Pipeline::enablePrimitiveRestart(const bool& enable){
		INFO.inputAssemblyInfo.primitiveRestartEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::enableDepthClamp(const bool& enable){
		INFO.rasterizationInfo.depthClampEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::enableRasterizerDiscard(const bool& enable){
		INFO.rasterizationInfo.rasterizerDiscardEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setPolygonMode(const PolygonMode& mode){
		INFO.rasterizationInfo.polygonMode = toVulkan(mode);
		return *this;
	}

	Pipeline& Pipeline::setCullMode(const CullMode& mode){
		INFO.rasterizationInfo.cullMode = toVulkan(mode.get());
		return *this;
	}

	Pipeline& Pipeline::setFrontFace(const FrontFace& frontFace){
		INFO.rasterizationInfo.frontFace = toVulkan(frontFace);
		return *this;
	}

	Pipeline& Pipeline::enableDepthBias(const bool& enable){
		INFO.rasterizationInfo.depthBiasEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasConstant(const float& constant){
		INFO.rasterizationInfo.depthBiasConstantFactor = constant;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasSlope(const float& slope){
		INFO.rasterizationInfo.depthBiasSlopeFactor = slope;
		return *this;
	}

	Pipeline& Pipeline::setDepthBiasClamp(const float& clamp){
		INFO.rasterizationInfo.depthBiasClamp = clamp;
		return *this;
	}

	Pipeline& Pipeline::setLineWidth(const float& width){
		INFO.rasterizationInfo.lineWidth = width;
		return *this;
	}

	Pipeline& Pipeline::setSampleCount(const SampleCount::Bits& count){
		INFO.multisampleInfo.rasterizationSamples = toVulkan(count);
		return *this;
	}

	Pipeline& Pipeline::enableSampleShading(const bool& enable){
		INFO.multisampleInfo.sampleShadingEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setMinimumSampleShading(const float& min){
		INFO.multisampleInfo.minSampleShading = min;
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToCoverage(const bool& enable){
		INFO.multisampleInfo.alphaToCoverageEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::enableAlphaToOne(const bool& enable){
		INFO.multisampleInfo.alphaToOneEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setColorBlendFlags(const ColorBlendFlags& flags){
		INFO.colorBlendInfo.flags = toVulkan(flags);
		return *this;
	}

	Pipeline& Pipeline::enableBlendLogicOperation(const bool& enable){
		INFO.colorBlendInfo.logicOpEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setBlendLogicOperation(const LogicOperator& op){
		INFO.colorBlendInfo.logicOp = toVulkan(op);
		return *this;
	}

	Pipeline& Pipeline::setBlendConstant(const Color::Components::Bits& component, const float& constant){
		std::size_t id=0;
		switch (component){
			case Color::Components::RED: id = 0; break;
			case Color::Components::GREEN: id = 1; break;
			case Color::Components::BLUE: id = 2; break;
			case Color::Components::ALPHA: id = 3; break;
		}

		INFO.colorBlendInfo.blendConstants[id] = constant;
		return *this;
	}

	Pipeline& Pipeline::enableDepthTest(const bool& enable){
		INFO.depthStencilInfo.depthTestEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::enableDepthWrite(const bool& enable){
		INFO.depthStencilInfo.depthWriteEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setDepthCompareOp(const CompareOperator& op){
		INFO.depthStencilInfo.depthCompareOp = toVulkan(op);
		return *this;
	}

	Pipeline& Pipeline::enableDepthBoundsTest(const bool& enable){
		INFO.depthStencilInfo.depthBoundsTestEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::enableStencilTest(const bool& enable){
		INFO.depthStencilInfo.stencilTestEnable = toVulkan(enable);
		return *this;
	}

	Pipeline& Pipeline::setMaxDepthBounds(const float& max){
		INFO.depthStencilInfo.maxDepthBounds = max;
		return *this;
	}

	Pipeline& Pipeline::setMinDepthBounds(const float& min){
		INFO.depthStencilInfo.maxDepthBounds = min;
		return *this;
	}

	Pipeline& Pipeline::addDynamicState(const DynamicState& state){
		_impl->info.dynamicStates.push_back(state);
		return *this;
	}

	Pipeline& Pipeline::addDynamicStates(const std::initializer_list<DynamicState>& states){
		for (auto& state : states){
			_impl->info.dynamicStates.push_back(state);
		}
		return *this;
	}

	Pipeline& Pipeline::setTellesationPatchControlPoints(const std::uint32_t& count){
		INFO.tessellationInfo.patchControlPoints = count;
		return *this;
	}

	Pipeline& Pipeline::addStage(const std::shared_ptr<Shader>& shader, const Shader::Stage::Bits& stage, const char* entryPoint, const Shader::Flags& flags){
		_impl->info.stages.push_back({shader, entryPoint, stage, flags});
		return *this;
	}

	Pipeline& Pipeline::setFrontStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.front = toVulkan(operations);
		return *this;
	}

	Pipeline& Pipeline::setBackStencilOpertions(const StencilOpState& operations){
		INFO.depthStencilInfo.back = toVulkan(operations);
		return *this;
	}
	

	void Pipeline::bind(CommandBuffer& commandBuffer){
		_impl->pipeline->bind(commandBuffer.getImpl()->commandBuffer);
	}

}