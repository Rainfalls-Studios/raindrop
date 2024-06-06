#ifndef __RAINDROP_PIPELINE_HPP__
#define __RAINDROP_PIPELINE_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"
#include "GUID.hpp"
#include "Format.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Asset.hpp"
#include "Operators.hpp"
#include "Vertex.hpp"

namespace Raindrop{
	class Pipeline{
		friend class VertexBinding;
		public:
			class Flags : public Utils::FlagsTemplate<Flags>{
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						DISABLE_OPTIMIZATION = 1 << 0,
						ALLOW_DERIVATIVES = 1 << 1,
						DERIVATIVE = 1 << 2,
						VIEW_INDEX_FROM_DEVICE_INDEX = 1 << 3,
						DISPATCH_BASE = 1 << 4,
						FAIL_ON_PIPELINE_COMPILE_REQUIRED = 1 << 5,
						EARLY_RETURN_ON_FAILURE = 1 << 6,
						RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT = 1 << 7,
						RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT = 1 << 8,
						RAY_TRACING_NO_NULL_ANY_HIT_SHADERS = 1 << 9,
						RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS = 1 << 10,
						RAY_TRACING_NO_NULL_MISS_SHADERS = 1 << 11,
						RAY_TRACING_NO_NULL_INTERSECTION_SHADERS = 1 << 12,
						RAY_TRACING_SKIP_TRIANGLES = 1 << 13,
						RAY_TRACING_SKIP_AABBS = 1 << 14,
						RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY = 1 << 15,
						DEFER_COMPILE = 1 << 16,
						CAPTURE_STATISTICS = 1 << 17,
						CAPTURE_INTERNAL_REPRESENTATIONS = 1 << 18,
						INDIRECT_BINDABLE = 1 << 19,
						LIBRARY = 1 << 20,
						DESCRIPTOR_BUFFER = 1 << 21,
						RETAIN_LINK_TIME_OPTIMIZATION_INFO = 1 << 22,
						LINK_TIME_OPTIMIZATION = 1 << 23,
						RAY_TRACING_ALLOW_MOTION = 1 << 24,
						COLOR_ATTACHMENT_FEEDBACK_LOOP = 1 << 25,
						DEPTH_STENCIL_ATTACHMENT_FEEDBACK_LOOP = 1 << 26,
						RAY_TRACING_OPACITY_MICROMAP = 1 << 27,
						NO_PROTECTED_ACCESS = 1 << 28,
						PROTECTED_ACCESS_ONLY = 1 << 29,
					};
			};

			class Stage : public Utils::FlagsTemplate<Stage>{
				public:
					using FlagsTemplate<Stage>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						TOP_OF_PIPE = 1 << 0,
						DRAW_INDIRECT = 1 << 1,
						VERTEX_INPUT = 1 << 2,
						VERTEX_SHADER = 1 << 3,
						TESSELLATION_CONTROL_SHADER = 1 << 4,
						TESSELLATION_EVALUATION_SHADER = 1 << 5,
						GEOMETRY_SHADER = 1 << 6,
						FRAGMENT_SHADER = 1 << 7,
						EARLY_FRAGMENT_TESTS = 1 << 8,
						LATE_FRAGMENT_TESTS = 1 << 9,
						COLOR_ATTACHMENT_OUTPUT = 1 << 10,
						COMPUTE_SHADER = 1 << 11,
						TRANSFER = 1 << 12,
						BOTTOM_OF_PIPE = 1 << 13,
						HOST = 1 << 14,
						ALL_GRAPHICS = 1 << 15,
						ALL_COMMANDS = 1 << 16,
						TRANSFORM_FEEDBACK = 1 << 17,
						CONDITIONAL_RENDERING = 1 << 18,
						ACCELERATION_STRUCTURE_BUILD = 1 << 19,
						RAY_TRACING_SHADER = 1 << 20,
						FRAGMENT_DENSITY_PROCESS = 1 << 21,
						FRAGMENT_SHADING_RATE_ATTACHMENT = 1 << 22,
						COMMAND_PREPROCES = 1 << 23,
						TASK_SHADER = 1 << 24,
						MESH_SHADER = 1 << 26,
					};
			};

			class ColorBlendFlags : public Utils::FlagsTemplate<ColorBlendFlags>{
				public:
					using FlagsTemplate<ColorBlendFlags>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						RASTERIZATION_ORDER_ATTACHMENT_ACCESS = 1 << 0
					};
			};

			class DepthStencilFlags : public Utils::FlagsTemplate<DepthStencilFlags>{
				public:
					using FlagsTemplate<DepthStencilFlags>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS = 1 << 0,
						RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS = 1 << 1,
					};
			};

			class CullMode : public Utils::FlagsTemplate<CullMode>{
				public:
					using FlagsTemplate<CullMode>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						FRONT = 1 << 0,
						BACK = 1 << 1,
					};
			};

			class SampleCount : public Utils::FlagsTemplate<SampleCount>{
				public:
					using FlagsTemplate<SampleCount>::FlagsTemplate;

					enum Bits : Bitset{
						ONE = 1 << 0,
						TWO = 1 << 1,
						FOUR = 1 << 2,
						HEIGHT = 1 << 3,
						SIXTEEN = 1 << 4,
						THIRTY_TWO = 1 << 5,
						SIXTY_FOUR = 1 << 6,
					};
			};

			enum class Topology{
				POINT_LIST,
				LINE_LIST,
				LINE_STRIP,
				TRIANGLE_LIST,
				TRIANGLE_STRIP,
				TRIANGLE_FAN,
				LINE_LIST_WITH_ADJACENCY,
				LINE_STRIP_WITH_ADJACENCY,
				TRIANGLE_LIST_WITH_ADJACENCY,
				TRIANGLE_STRIP_WITH_ADJACENCY,
				PATCH_LIST
			};

			enum class PolygonMode{
				FILL,
				LINE,
				POINT,
				FILL_RECTANGLE
			};

			enum class FrontFace{
				COUNTER_CLOCKWISE = 0,
				CLOCKWISE = 1,
			};

			enum class StencilOperation{
				KEEP,
				ZERO,
				REPLACE,
				INCREMENT_AND_CLAMP,
				DECREMENT_AND_CLAMP,
				INVERT,
				INCREMENT_AND_WRAP,
				DECREMENT_AND_WRAP,
			};

			enum class DynamicState{
				VIEWPORT,
				SCISSOR,
				LINE_WIDTH,
				DEPTH_BIAS,
				BLEND_CONSTANTS,
				DEPTH_BOUNDS,
				STENCIL_COMPARE_MASK,
				STENCIL_WRITE_MASK,
				STENCIL_REFERENCE,
				CULL_MODE,
				FRONT_FACE,
				PRIMITIVE_TOPOLOGY,
				VIEWPORT_WITH_COUNT,
				SCISSOR_WITH_COUNT,
				VERTEX_INPUT_BINDING_STRIDE,
				DEPTH_TEST_ENABLE,
				DEPTH_WRITE_ENABLE,
				DEPTH_COMPARE_OP,
				DEPTH_BOUNDS_TEST_ENABLE,
				STENCIL_TEST_ENABLE,
				STENCIL_OP,
				RASTERIZER_DISCARD_ENABLE,
				DEPTH_BIAS_ENABLE,
				PRIMITIVE_RESTART_ENABLE,
				VIEWPORT_W_SCALING,
				DISCARD_RECTANGLE,
				DISCARD_RECTANGLE_ENABLE,
				DISCARD_RECTANGLE_MODE,
				SAMPLE_LOCATIONS,
				RAY_TRACING_PIPELINE_STACK_SIZE,
				VIEWPORT_SHADING_RATE_PALETTE,
				VIEWPORT_COARSE_SAMPLE_ORDER,
				EXCLUSIVE_SCISSOR_ENABLE,
				EXCLUSIVE_SCISSOR,
				FRAGMENT_SHADING_RATE,
				VERTEX_INPUT,
				PATCH_CONTROL_POINTS,
				LOGIC_OP,
				COLOR_WRITE_ENABLE,
				DEPTH_CLAMP_ENABLE,
				POLYGON_MODE,
				RASTERIZATION_SAMPLES,
				SAMPLE_MASK,
				ALPHA_TO_COVERAGE_ENABLE,
				ALPHA_TO_ONE_ENABLE,
				LOGIC_OP_ENABLE,
				COLOR_BLEND_ENABLE,
				COLOR_BLEND_EQUATION,
				COLOR_WRITE_MASK,
				TESSELLATION_DOMAIN_ORIGIN,
				RASTERIZATION_STREAM,
				CONSERVATIVE_RASTERIZATION_MODE,
				EXTRA_PRIMITIVE_OVERESTIMATION_SIZE,
				DEPTH_CLIP_ENABLE,
				SAMPLE_LOCATIONS_ENABLE,
				COLOR_BLEND_ADVANCED,
				PROVOKING_VERTEX_MODE,
				LINE_RASTERIZATION_MODE,
				LINE_STIPPLE_ENABLE,
				DEPTH_CLIP_NEGATIVE_ONE_TO_ONE,
				VIEWPORT_W_SCALING_ENABLE,
				VIEWPORT_SWIZZLE,
				COVERAGE_TO_COLOR_ENABLE,
				COVERAGE_TO_COLOR_LOCATION,
				COVERAGE_MODULATION_MODE,
				COVERAGE_MODULATION_TABLE_ENABLE,
				COVERAGE_MODULATION_TABLE,
				SHADING_RATE_IMAGE_ENABLE,
				REPRESENTATIVE_FRAGMENT_TEST_ENABLE,
				COVERAGE_REDUCTION_MODE,
				ATTACHMENT_FEEDBACK_LOOP_ENABLE,
				LINE_STIPPLE,
			};

			class Viewport{
				friend class Pipeline;
				public:
					Viewport& setX(const float& x) noexcept;
					Viewport& setY(const float& y) noexcept;
					Viewport& setWidth(const float& width) noexcept;
					Viewport& setHeight(const float& height) noexcept;
					Viewport& setMinDepth(const float& minDepth) noexcept;
					Viewport& setMaxDepth(const float& maxDepth) noexcept;

					const float& getX() const noexcept;
					const float& getY() const noexcept;
					const float& getWidth() const noexcept;
					const float& getHeight() const noexcept;
					const float& getMinDepth() const noexcept;
					const float& getMaxDepth() const noexcept;
					
				private:
					float _x;
					float _y;
					float _width;
					float _height;
					float _minDepth;
					float _maxDepth;

			};

			class Scissor{
				friend class Pipeline;
				public:
					Scissor& setX(const std::int32_t& x) noexcept;
					Scissor& setY(const std::int32_t& y) noexcept;
					Scissor& setWidth(const std::uint32_t& width) noexcept;
					Scissor& setHeight(const std::uint32_t& height) noexcept;

					const std::int32_t& getX() const noexcept;
					const std::int32_t& getY() const noexcept;
					const std::uint32_t& getWidth() const noexcept;
					const std::uint32_t& getHeight() const noexcept;
				
				private:
					std::int32_t _x;
					std::int32_t _y;
					std::uint32_t _width;
					std::uint32_t _height;
			};

			class ColorAttachment{
				friend class Pipeline;
				public:
					ColorAttachment() noexcept;

					ColorAttachment& enableBlending(const bool& enable = true) noexcept;
					ColorAttachment& setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept;
					ColorAttachment& setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept;
					ColorAttachment& setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept;
					ColorAttachment& setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept;
					ColorAttachment& setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept;
					ColorAttachment& setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept;
					ColorAttachment& setWriteMask(const Color::Components& writeMask) noexcept;


					const bool& isBlendingEnable() const noexcept;
					const Color::BlendFactor& getSrcColorBlendFactor() const noexcept;
					const Color::BlendFactor& getDstColorBlendFactor() const noexcept;
					const Color::BlendOperation& getColorBlendOp() const noexcept;
					const Color::BlendFactor& getSrcAlphaBlendFactor() const noexcept;
					const Color::BlendFactor& getDstAlphaBlendFactor() const noexcept;
					const Color::BlendOperation& getAlphaBlendOp() const noexcept;
					const Color::Components& getWriteMask() const noexcept;

				private:
					bool _blendEnabled;
					Color::BlendFactor _srcColorBlendFactor;
					Color::BlendFactor _dstColorBlendFactor;
					Color::BlendOperation _colorBlendOp;
					Color::BlendFactor _srcAlphaBlendFactor;
					Color::BlendFactor _dstAlphaBlendFactor;
					Color::BlendOperation _alphaBlendOp;
					Color::Components _writeMask;

			};

			class StencilOpState{
				friend class Pipeline;
				public:
					StencilOpState& setFailOp(const StencilOperation& failOp) noexcept;
					StencilOpState& setPassOp(const StencilOperation& passOp) noexcept;
					StencilOpState& setDepthFailOp(const StencilOperation& depthFailOp) noexcept;
					StencilOpState& setCompareOp(const CompareOperator& compareOp) noexcept;
					StencilOpState& setCompareMask(const std::uint32_t& compareMask) noexcept;
					StencilOpState& setWriteMask(const std::uint32_t& writeMask) noexcept;
					StencilOpState& setReference(const std::uint32_t& reference) noexcept;

					const StencilOperation& getFailOp() const noexcept;
					const StencilOperation& getPassOp() const noexcept;
					const StencilOperation& getDepthFailOp() const noexcept;
					const CompareOperator& getCompareOp() const noexcept;
					const std::uint32_t& getCompareMask() const noexcept;
					const std::uint32_t& getWriteMask() const noexcept;
					const std::uint32_t& getReference() const noexcept;

				private:
					StencilOperation _failOp;
					StencilOperation _passOp;
					StencilOperation _depthFailOp;
					CompareOperator _compareOp;
					std::uint32_t _compareMask;
					std::uint32_t _writeMask;
					std::uint32_t _reference;
			};

			class Layout{
				public:
					class Flags : public Utils::FlagsTemplate<Flags>{
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits : Bitset{
								NONE = 0,
								INDEPENDENT_SETS = 1 << 1
							};
					};

					struct Impl;

					static Layout Create(Context& context);

					Layout(Context& context);
					~Layout();

					Layout(const Layout& other);
					Layout& operator=(const Layout& other);


					void initialize();
					void release();

					void setFlags(const Flags& flags);

					void setPushConstant(const Stage& stage, const std::size_t size);

					template<typename T>
					inline void setPushConstant(const Stage& stage){
						setPushConstant(stage, sizeof(T));
					}

					// void setSetLayouts();
					// void addSetLayout();

					Flags getFlags() const noexcept;
					// void getPushConstants();
					// void getSetLayouts();


					bool isInitialized() const noexcept;
					void* getNativeHandle() const;
					Impl* getImpl() const noexcept;
					GUID getGUID() const noexcept;

				private:
					Impl* _impl;
			};

			class Shader : public Asset{
				public:
					struct Impl;

					class Flags : public Utils::FlagsTemplate<Flags>{
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits : Bitset{
								NONE = 0,
								ALLOW_VARYING_SUBGROUP_SIZE = 1 << 1,
								REQUIRE_FULL_SUBGROUPS = 1 << 2,
							};
					};

					class Stage : public Utils::FlagsTemplate<Stage>{
						public:
							using FlagsTemplate<Stage>::FlagsTemplate;

							enum Bits : Bitset{
								NONE = 0,
								VERTEX = 1 << 0,
								TESSELLATION_CONTROL = 1 << 1,
								TESSELLATION_EVALUATION = 1 << 2,
								GEOMETRY = 1 << 3,
								FRAGMENT = 1 << 4,
								COMPUTE = 1 << 5,
								ALL_GRAPHICS = 1 << 6,
								RAYGEN = 1 << 7,
								ANY_HIT = 1 << 8,
								CLOSEST_HIT = 1 << 9,
								MISS = 1 << 10,
								INTERSECTION = 1 << 11,
								CALLABLE = 1 << 12,
								TASK = 1 << 13,
								MESH = 1 << 14,
								SUBPASS_SHADING = 1 << 15,
								CLUSTER_CULLING = 1 << 16,
							};
					};

					static Shader Create(Context& context, const Path& path);
					static std::shared_ptr<Shader> Load(Context& context, const Path& path);

					Shader(Context& context, const Path& path);
					~Shader();

					void* getNativeHandle() const;
					Impl* getImpl() const noexcept;
					bool isInitialized() const noexcept;

				private:
					Impl* _impl;
			};

			struct Impl;

			static Pipeline Create(Context& context);

			Pipeline(Context& context);
			~Pipeline();

			Pipeline(const Pipeline& other);
			Pipeline& operator=(const Pipeline& other);

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;


			Pipeline& setFlags(const Flags& flags);
			Pipeline& setRenderPass(const RenderPass& renderPass);
			Pipeline& setLayout(const Layout& layout);

			Pipeline& setPrimitiveTopology(const Topology& topology);
			Pipeline& enablePrimitiveRestart(const bool& enable = true);

			Pipeline& enableDepthClamp(const bool& enable = true);
			Pipeline& enableRasterizerDiscard(const bool& enable = true);
			Pipeline& setPolygonMode(const PolygonMode& mode);
			Pipeline& setCullMode(const CullMode& mode);
			Pipeline& setFrontFace(const FrontFace& frontFace);
			Pipeline& enableDepthBias(const bool& enable = true);
			Pipeline& setDepthBiasConstant(const float& constant);
			Pipeline& setDepthBiasSlope(const float& slope);
			Pipeline& setDepthBiasClamp(const float& clamp);
			Pipeline& setLineWidth(const float& width);

			Pipeline& setSampleCount(const SampleCount::Bits& count);
			Pipeline& enableSampleShading(const bool& enable = true);
			Pipeline& setMinimumSampleShading(const float& min);
			Pipeline& enableAlphaToCoverage(const bool& enable = true);
			Pipeline& enableAlphaToOne(const bool& enable = true);
			// TODO: sample mask

			Pipeline& setColorBlendFlags(const ColorBlendFlags& flags);
			Pipeline& enableBlendLogicOperation(const bool& enable = true);
			Pipeline& setBlendLogicOperation(const LogicOperator& op);
			Pipeline& setBlendConstant(const Color::Components::Bits& component, const float& constant);

			Pipeline& enableDepthTest(const bool& enable = true);
			Pipeline& enableDepthWrite(const bool& enable = true);
			Pipeline& setDepthCompareOp(const CompareOperator& op);
			Pipeline& enableDepthBoundsTest(const bool& enable);
			Pipeline& enableStencilTest(const bool& enable);
			Pipeline& setFrontStencilOpertions(const StencilOpState& operations);
			Pipeline& setBackStencilOpertions(const StencilOpState& operations);
			Pipeline& setMaxDepthBounds(const float& max);
			Pipeline& setMinDepthBounds(const float& min);

			Pipeline& addDynamicState(const DynamicState& state);
			Pipeline& addDynamicStates(const std::initializer_list<DynamicState>& states);

			Pipeline& setTellesationPatchControlPoints(const std::uint32_t& count);

			Pipeline& addStage(const std::shared_ptr<Shader>& shader, const Shader::Stage::Bits& stage, const char* entryPoint = "main", const Shader::Flags& flags = Shader::Flags::NONE);
			ColorAttachment& addColorAttachment();
			Vertex::Layout& addVertexBinding(const Vertex::InputRate& inputRate);
			Viewport& addViewport();
			Scissor& addScissor();


			void bind(CommandBuffer& commandbuffer);

		private:
			Impl* _impl;				
	};

	static inline Pipeline CreatePipeline(Context& context){
		return Pipeline::Create(context);
	}

	static inline Pipeline::Layout CreatePipelineLayout(Context& context){
		return Pipeline::Layout::Create(context);
	}

	static inline Pipeline::Shader Create(Context& context, const Path& path){
		return Pipeline::Shader::Create(context, path);
	}

	static inline std::shared_ptr<Pipeline::Shader> Load(Context& context, const Path& path){
		return Pipeline::Shader::Load(context, path);
	}
}

#endif