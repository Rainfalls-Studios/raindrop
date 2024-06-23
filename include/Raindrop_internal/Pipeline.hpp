#ifndef __RAINDROP_INTERNAL_PIPELINE_HPP__
#define __RAINDROP_INTERNAL_PIPELINE_HPP__

#include <Raindrop/Pipeline.hpp>
#include <Raindrop/Context.hpp>
#include <Raindrop_internal/Graphics/GraphicsPipeline.hpp>
#include <Raindrop_internal/Graphics/PipelineLayout.hpp>
#include <Raindrop_internal/Graphics/Shader.hpp>
#include <Raindrop_internal/Color.hpp>
#include <Raindrop_internal/Operators.hpp>

namespace Raindrop{
	// TODO: make infos temporary -> destroy on resource initialization

	struct Pipeline::Layout::Impl{
		Internal::Context* context;
		Flags flags;
		std::optional<PushConstantDescription> pushConstantDescription;

		std::shared_ptr<Internal::Graphics::PipelineLayout> internal;


		Impl() : 
			context{nullptr},
			internal{}
		{}
	};

	struct Pipeline::Shader::Impl{
		Internal::Context* context;

		std::shared_ptr<Internal::Graphics::Shader> internal;

		Impl() : 
			context{nullptr},
			internal{}
		{}
	};

	struct Pipeline::Impl{
		struct Stage{
			std::shared_ptr<Shader> shader;
			std::string entryPoint;
			Shader::Stage::Bits stage;
			Shader::Flags flags;
		};

		Internal::Context* context;
		const RenderPass* renderPass;
		const Layout* layout;

		std::vector<ColorAttachment> attachments;
		std::vector<std::pair<Vertex::Layout, Vertex::InputRate>> bindings;
		std::vector<Viewport> viewports;
		std::vector<Scissor> scissors;
		std::vector<DynamicState> dynamicStates;
		std::vector<Stage> stages;

		Flags flags;
		uint32_t subpass;
		Topology primitiveTopology;
		PolygonMode polygonMode;
		CullMode cullMode;
		FrontFace frontFace;
		SampleCount::Bits sampleCount;
		ColorBlendFlags colorBlendFlags;
		LogicOperator blendLogicOperation;
		CompareOperator depthCompareOperation;
		uint32_t tessellationPatchControlPoints;
		StencilOpState frontStencilOpertions;
		StencilOpState backStencilOpertions;

		float depthBiasConstantFactor;
		float depthBiasSlopeFactor;
		float depthBiasClamp;
		float lineWidth;
		float minSampleShading;
		float blendConstants[4];
		float maxDepthBounds;
		float minDepthBounds;

		bool primitiveRestartEnable;
		bool depthClampEnable;
		bool rasterizationDiscardEnable;
		bool depthBiasEnable;
		bool sampleShadingEnable;
		bool alphaToCoverageEnable;
		bool alphaToOneEnable;
		bool blendLogicOpertionEnable;
		bool depthTestEnable;
		bool depthStencilEnable;
		bool depthWriteEnable;
		bool depthBoundsTestEnable;
		bool stencilTestEnable;

		std::shared_ptr<Internal::Graphics::GraphicsPipeline> internal;

		Impl() :
			context{nullptr},
			attachments{},
			bindings{},
			viewports{},
			scissors{},
			dynamicStates{},
			stages{},
			renderPass{},
			layout{},

			flags{Flags::NONE},
			subpass{0},
			primitiveTopology{Topology::TRIANGLE_LIST},
			polygonMode{PolygonMode::FILL},
			cullMode{CullMode::NONE},
			frontFace{FrontFace::CLOCKWISE},
			sampleCount{SampleCount::ONE},
			colorBlendFlags(ColorBlendFlags::NONE),
			blendLogicOperation{LogicOperator::AND},
			depthCompareOperation{CompareOperator::LESS},
			tessellationPatchControlPoints{0},
			frontStencilOpertions(),
			backStencilOpertions(),

			depthBiasConstantFactor{0.f},
			depthBiasSlopeFactor{1.f},
			depthBiasClamp{1.f},
			lineWidth{1.f},
			minSampleShading{0.f},
			blendConstants{1.f, 1.f, 1.f, 1.f},
			maxDepthBounds{1.f},
			minDepthBounds{0.f},

			primitiveRestartEnable{false},
			depthClampEnable{false},
			rasterizationDiscardEnable{false},
			depthBiasEnable{false},
			sampleShadingEnable{false},
			alphaToCoverageEnable{false},
			alphaToOneEnable{false},
			blendLogicOpertionEnable{false},
			depthTestEnable{true},
			depthWriteEnable{true},
			depthBoundsTestEnable{false},
			stencilTestEnable{false},

			internal{}
		{}
	};

	constexpr VkPipelineCreateFlags PipelineCreateFlagsToVulkan(const Pipeline::Flags& flags){
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

	constexpr VkPipelineStageFlags pipelineStageFlagsToVulkan(const Pipeline::Stage& stage){
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

	constexpr VkPipelineColorBlendStateCreateFlags pipelineColorBlendStateCreateFlagsToVulkan(const Pipeline::ColorBlendFlags& flags){
		VkPipelineColorBlendStateCreateFlags out = 0;
		
		// VK_EXT_rasterization_order_attachment_access
		if (flags.has(Pipeline::ColorBlendFlags::RASTERIZATION_ORDER_ATTACHMENT_ACCESS)) out |= VK_PIPELINE_COLOR_BLEND_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_ACCESS_BIT_EXT;

		return out;
	}

	constexpr VkPipelineDepthStencilStateCreateFlags pipelineDepthStencilStateCreateFlagsToVulkan(const Pipeline::DepthStencilFlags& flags){
		VkPipelineDepthStencilStateCreateFlags out = 0;

		// VK_EXT_rasterization_order_attachment_access
		if (flags.has(Pipeline::DepthStencilFlags::RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS)) out |= VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS_BIT_EXT;
		if (flags.has(Pipeline::DepthStencilFlags::RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS)) out |= VK_PIPELINE_DEPTH_STENCIL_STATE_CREATE_RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS_BIT_EXT;

		return out;
	}

	constexpr VkCullModeFlags cullModeFlagsToVulkan(const Pipeline::CullMode& mode){
		VkCullModeFlags out = 0;

		if (mode.has(Pipeline::CullMode::FRONT)) out |= VK_CULL_MODE_FRONT_BIT;
		if (mode.has(Pipeline::CullMode::BACK)) out |= VK_CULL_MODE_BACK_BIT;

		return out;
	}

	constexpr VkPrimitiveTopology primitiveTopologyToVulkan(const Pipeline::Topology& t){
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

	constexpr VkPolygonMode polygonModeToVulkan(const Pipeline::PolygonMode& mode){
		switch (mode){
			case Pipeline::PolygonMode::FILL: return VK_POLYGON_MODE_FILL;
			case Pipeline::PolygonMode::LINE: return VK_POLYGON_MODE_LINE;
			case Pipeline::PolygonMode::POINT: return VK_POLYGON_MODE_POINT;

			// VK_NV_fill_rectangle
			case Pipeline::PolygonMode::FILL_RECTANGLE: return VK_POLYGON_MODE_FILL_RECTANGLE_NV;
		}
		throw std::runtime_error("Invalid polygon mode");
	}

	constexpr VkFrontFace frontFaceToVulkan(const Pipeline::FrontFace& f){
		switch (f){
			case Pipeline::FrontFace::COUNTER_CLOCKWISE: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			case Pipeline::FrontFace::CLOCKWISE: return VK_FRONT_FACE_CLOCKWISE;
		}
		throw std::runtime_error("Invalid front face");
	}

	constexpr VkSampleCountFlags sampleCountFlagsToVulkan(const Pipeline::SampleCount& count){
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

	constexpr VkSampleCountFlagBits sampleCountFlagsBitsToVulkan(const Pipeline::SampleCount::Bits& count){
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

	constexpr VkStencilOp stencilOpToVulkan(const Pipeline::StencilOperation& op){
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

	constexpr VkDynamicState dynamicStateToVulkan(const Pipeline::DynamicState& s){
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
		}
		throw std::runtime_error("Invalid dynamic state");
	}

	constexpr VkPipelineShaderStageCreateFlags pipelineShaderStageCreateFlagsToVulkan(const Pipeline::Shader::Flags& f){
		VkPipelineShaderStageCreateFlags out = 0;

		if (f.has(Pipeline::Shader::Flags::ALLOW_VARYING_SUBGROUP_SIZE)) out |= VK_PIPELINE_SHADER_STAGE_CREATE_ALLOW_VARYING_SUBGROUP_SIZE_BIT;
		if (f.has(Pipeline::Shader::Flags::REQUIRE_FULL_SUBGROUPS)) out |= VK_PIPELINE_SHADER_STAGE_CREATE_REQUIRE_FULL_SUBGROUPS_BIT;

		return out;
	}

	constexpr VkShaderStageFlags shaderStageFlagsToVulkan(const Pipeline::Shader::Stage& s){
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

	constexpr VkShaderStageFlagBits shaderStageFlagsBits(const Pipeline::Shader::Stage::Bits& s){
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

	constexpr VkPipelineLayoutCreateFlags pipelineLayoutFlagsToVulkan(const Pipeline::Layout::Flags& f){
		VkPipelineLayoutCreateFlags out = 0;
		if (f.has(Pipeline::Layout::Flags::INDEPENDENT_SETS)) out |= VK_PIPELINE_LAYOUT_CREATE_INDEPENDENT_SETS_BIT_EXT;

		return out;
	}

	VkStencilOpState StencilOpStateToVulkan(const Pipeline::StencilOpState& state);
}

#endif