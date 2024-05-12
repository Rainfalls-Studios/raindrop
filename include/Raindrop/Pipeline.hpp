#ifndef __RAINDROP_PIPELINE_HPP__
#define __RAINDROP_PIPELINE_HPP__

#include "common.hpp"
#include "Utils/Flags.hpp"
#include "GUID.hpp"
#include "Format.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Asset.hpp"

namespace Raindrop{
	class Pipeline{
		public:
			class Flags : public Utils::FlagsTemplate<Flags>{
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						DISABLE_OPTIMIZATION = 0x00000001,
						ALLOW_DERIVATIVES = 0x00000002,
						DERIVATIVE = 0x00000004,
						VIEW_INDEX_FROM_DEVICE_INDEX = 0x00000008,
						DISPATCH_BASE = 0x00000010,
						FAIL_ON_PIPELINE_COMPILE_REQUIRED = 0x00000100,
						EARLY_RETURN_ON_FAILURE = 0x00000200,
						RENDERING_FRAGMENT_SHADING_RATE_ATTACHMENT = 0x00200000,
						RENDERING_FRAGMENT_DENSITY_MAP_ATTACHMENT = 0x00400000,
						RAY_TRACING_NO_NULL_ANY_HIT_SHADERS = 0x00004000,
						RAY_TRACING_NO_NULL_CLOSEST_HIT_SHADERS = 0x00008000,
						RAY_TRACING_NO_NULL_MISS_SHADERS = 0x00010000,
						RAY_TRACING_NO_NULL_INTERSECTION_SHADERS = 0x00020000,
						RAY_TRACING_SKIP_TRIANGLES = 0x00001000,
						RAY_TRACING_SKIP_AABBS = 0x00002000,
						RAY_TRACING_SHADER_GROUP_HANDLE_CAPTURE_REPLAY = 0x00080000,
						DEFER_COMPILE = 0x00000020,
						CAPTURE_STATISTICS = 0x00000040,
						CAPTURE_INTERNAL_REPRESENTATIONS = 0x00000080,
						INDIRECT_BINDABLE = 0x00040000,
						LIBRARY = 0x00000800,
						DESCRIPTOR_BUFFER = 0x20000000,
						RETAIN_LINK_TIME_OPTIMIZATION_INFO = 0x00800000,
						LINK_TIME_OPTIMIZATION = 0x00000400,
						RAY_TRACING_ALLOW_MOTION = 0x00100000,
						COLOR_ATTACHMENT_FEEDBACK_LOOP = 0x02000000,
						DEPTH_STENCIL_ATTACHMENT_FEEDBACK_LOOP = 0x04000000,
						RAY_TRACING_OPACITY_MICROMAP = 0x01000000,
						VK_PIPELINE_CREATE_NO_PROTECTED_ACCESS = 0x08000000,
						VK_PIPELINE_CREATE_PROTECTED_ACCESS_ONLY_ = 0x40000000,
					};
			};

			enum class InputRate{
				VERTEX,
				INSTANCE
			};

			class VertexBinding{
				public:
					class VertexAttribute{
						public:
							VertexAttribute() noexcept;
							VertexAttribute(const std::size_t& location, const std::size_t& offset, const Format& format) noexcept;

							void setLocation(const std::size_t& location) noexcept;
							void setOffset(const std::size_t& offset) noexcept;
							void setFormat(const Format& format) noexcept;

							const std::size_t& getLocation() const noexcept;
							const std::size_t& getOffset() const noexcept;
							const Format& getFormat() const noexcept;
						private:
							std::size_t _location;
							std::size_t _offset;
							Format _format;
					};

					VertexBinding() noexcept;
					VertexBinding(const std::size_t& binding, const std::initializer_list<VertexAttribute>& atrtibutes = {});

					void setBinding(const std::size_t& binding) noexcept;
					void setInputRate(const InputRate& rate) noexcept;
					void setStride(const std::size_t stride) noexcept;
					void addAttribute(const VertexAttribute& attribute);
					void setAttributes(const std::initializer_list<VertexAttribute>& attributes);

					void deduceStride();

					const std::size_t& getBinding() const noexcept;
					const std::size_t& getStride() const noexcept;
					const InputRate& getInputRate() const noexcept;
					const std::vector<VertexAttribute>& getAttributes() const noexcept;

				private:
					std::vector<VertexAttribute> _attributes;
					std::size_t _binding;
					InputRate _inputRate;
					std::size_t _stride;

					void checkLocations();
			};

			class Viewport{
				public:
					Viewport() noexcept;

					void setX(const float& x) noexcept;
					void setY(const float& y) noexcept;
					void setWidth(const float& width) noexcept;
					void setHeight(const float& height) noexcept;
					void setMinDepth(const float& minDepth) noexcept;
					void setMaxDepth(const float& maxDepth) noexcept;

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
				public:
					Scissor() noexcept;

					void setX(const std::size_t& x) noexcept;
					void setY(const std::size_t& y) noexcept;
					void setWidth(const std::size_t& width) noexcept;
					void setHeight(const std::size_t& height) noexcept;

					const std::size_t& getX() const noexcept;
					const std::size_t& getY() const noexcept;
					const std::size_t& getWidth() const noexcept;
					const std::size_t& getHeight() const noexcept;
				
				private:
					std::size_t _x;
					std::size_t _y;
					std::size_t _width;
					std::size_t _height;
			};

			enum class Topology{
				POINT_LIST = 0,
				LINE_LIST = 1,
				LINE_STRIP = 2,
				TRIANGLE_LIST = 3,
				TRIANGLE_STRIP = 4,
				TRIANGLE_FAN = 5,
				LINE_LIST_WITH_ADJACENCY = 6,
				LINE_STRIP_WITH_ADJACENCY = 7,
				TRIANGLE_LIST_WITH_ADJACENCY = 8,
				TRIANGLE_STRIP_WITH_ADJACENCY = 9,
				PATCH_LIST = 10
			};

			enum class PolygonMode{
				FILL = 0,
				LINE = 1,
				POINT = 2,
				FILL_RECTANGLE_NV = 1000153000
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

			enum class FrontFace{
				COUNTER_CLOCKWISE = 0,
				CLOCKWISE = 1,
			};

			// TODO: move out
			enum class SampleCount{
				ONE = 1 << 0,
				TWO = 1 << 1,
				FOUR = 1 << 2,
				HEIGHT = 1 << 3,
				SIXTEEN = 1 << 4,
				THIRTY_TWO = 1 << 5,
				SIXTY_FOUR = 1 << 6,
			};

			class ColorAttachment{
				public:
					ColorAttachment() noexcept;

					void enableBlending(const bool& enable = true) noexcept;
					void setSrcColorBlendFactor(const Color::BlendFactor& srcColorBlendFactor) noexcept;
					void setDstColorBlendFactor(const Color::BlendFactor& dstColorBlendFactor) noexcept;
					void setColorBlendOp(const Color::BlendOperation& colorBlendOp) noexcept;
					void setSrcAlphaBlendFactor(const Color::BlendFactor& srcAlphaBlendFactor) noexcept;
					void setDstAlphaBlendFactor(const Color::BlendFactor& dstAlphaBlendFactor) noexcept;
					void setAlphaBlendOp(const Color::BlendOperation& alphaBlendOp) noexcept;
					void setWriteMask(const Color::Components& writeMask) noexcept;

					bool isBlendingEnabled() const noexcept;
					const Color::BlendFactor& getSrcColorBlendFactor() const noexcept;
					const Color::BlendFactor& getDstColorBlendFactor() const noexcept;
					const Color::BlendOperation& getColorBlendOp() const noexcept;
					const Color::BlendFactor& getSrcAlphaBlendFactor() const noexcept;
					const Color::BlendFactor& getDstAlphaBlendFactor() const noexcept;
					const Color::BlendOperation& getAlphaBlendOp() const noexcept;
					const Color::Components& getWriteMask() const noexcept;

				private:
					bool _blendEnable;
					Color::BlendFactor _srcColorBlendFactor;
					Color::BlendFactor _dstColorBlendFactor;
					Color::BlendOperation _colorBlendOp;
					Color::BlendFactor _srcAlphaBlendFactor;
					Color::BlendFactor _dstAlphaBlendFactor;
					Color::BlendOperation _alphaBlendOp;
					Color::Components _writeMask;
			};


			class ColorBlendFlags : public Utils::FlagsTemplate<ColorBlendFlags>{
				public:
					using FlagsTemplate<ColorBlendFlags>::FlagsTemplate;

					enum Bits : Bitset{
						NONE = 0,
						RASTERIZATION_ORDER_ATTACHMENT_ACCESS = 1 << 0
					};
			};

			// TODO: move out
			enum class LogicOp{
				CLEAR = 0,
				AND = 1,
				AND_REVERSE = 2,
				COPY = 3,
				AND_INVERTED = 4,
				NO_OP = 5,
				XOR = 6,
				OR = 7,
				NOR = 8,
				EQUIVALENT = 9,
				INVERT = 10,
				OR_REVERSE = 11,
				COPY_INVERTED = 12,
				OR_INVERTED = 13,
				NAND = 14,
				SET = 15
			};

			// TODO: move out
			enum class CompareOp{
				NEVER = 0,
				LESS = 1,
				EQUAL = 2,
				LESS_OR_EQUAL = 3,
				GREATER = 4,
				NOT_EQUAL = 5,
				GREATER_OR_EQUAL = 6,
				ALWAYS = 7,
			};

			enum class StencilOp{
				KEEP = 0,
				ZERO = 1,
				REPLACE = 2,
				INCREMENT_AND_CLAMP = 3,
				DECREMENT_AND_CLAMP = 4,
				INVERT = 5,
				INCREMENT_AND_WRAP = 6,
				DECREMENT_AND_WRAP = 7,
			};

			class StencilOpState{
				public:
					StencilOpState() noexcept;

					void setFailOp(const StencilOp& failOp) noexcept;
					void setPassOp(const StencilOp& passOp) noexcept;
					void setDepthFailOp(const StencilOp& depthFailOp) noexcept;
					void setCompareOp(const CompareOp& compareOp) noexcept;
					void setComparMask(const std::uint32_t& comparMask) noexcept;
					void setWriteMask(const std::uint32_t& writeMask) noexcept;
					void setReference(const std::size_t& reference) noexcept;

					const StencilOp& getFailOp() const noexcept;
					const StencilOp& getPassOp() const noexcept;
					const StencilOp& getDepthFailOp() const noexcept;
					const CompareOp& getCompareOp() const noexcept;
					const std::uint32_t& getComparMask() const noexcept;
					const std::uint32_t& getWriteMask() const noexcept;
					const std::size_t& getReference() const noexcept;

				private:
					StencilOp _failOp;
					StencilOp _passOp;
					StencilOp _depthFailOp;
					CompareOp _compareOp;
					std::uint32_t _comparMask;
					std::uint32_t _writeMask;
					std::size_t _reference;
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

			enum class DynamicState{
				VIEWPORT = 0,
				SCISSOR = 1,
				LINE_WIDTH = 2,
				DEPTH_BIAS = 3,
				BLEND_CONSTANTS = 4,
				DEPTH_BOUNDS = 5,
				STENCIL_COMPARE_MASK = 6,
				STENCIL_WRITE_MASK = 7,
				STENCIL_REFERENCE = 8,
				CULL_MODE = 1000267000,
				FRONT_FACE = 1000267001,
				PRIMITIVE_TOPOLOGY = 1000267002,
				VIEWPORT_WITH_COUNT = 1000267003,
				SCISSOR_WITH_COUNT = 1000267004,
				VERTEX_INPUT_BINDING_STRIDE = 1000267005,
				DEPTH_TEST_ENABLE = 1000267006,
				DEPTH_WRITE_ENABLE = 1000267007,
				DEPTH_COMPARE_OP = 1000267008,
				DEPTH_BOUNDS_TEST_ENABLE = 1000267009,
				STENCIL_TEST_ENABLE = 1000267010,
				STENCIL_OP = 1000267011,
				RASTERIZER_DISCARD_ENABLE = 1000377001,
				DEPTH_BIAS_ENABLE = 1000377002,
				PRIMITIVE_RESTART_ENABLE = 1000377004,
				VIEWPORT_W_SCALING = 1000087000,
				DISCARD_RECTANGLE = 1000099000,
				DISCARD_RECTANGLE_ENABLE = 1000099001,
				DISCARD_RECTANGLE_MODE = 1000099002,
				SAMPLE_LOCATIONS = 1000143000,
				RAY_TRACING_PIPELINE_STACK_SIZE = 1000347000,
				VIEWPORT_SHADING_RATE_PALETTE = 1000164004,
				VIEWPORT_COARSE_SAMPLE_ORDER = 1000164006,
				EXCLUSIVE_SCISSOR_ENABLE = 1000205000,
				EXCLUSIVE_SCISSOR = 1000205001,
				FRAGMENT_SHADING_RATE = 1000226000,
				VERTEX_INPUT = 1000352000,
				PATCH_CONTROL_POINTS = 1000377000,
				LOGIC_OP = 1000377003,
				COLOR_WRITE_ENABLE = 1000381000,
				DEPTH_CLAMP_ENABLE = 1000455003,
				POLYGON_MODE = 1000455004,
				RASTERIZATION_SAMPLES = 1000455005,
				SAMPLE_MASK = 1000455006,
				ALPHA_TO_COVERAGE_ENABLE = 1000455007,
				ALPHA_TO_ONE_ENABLE = 1000455008,
				LOGIC_OP_ENABLE = 1000455009,
				COLOR_BLEND_ENABLE = 1000455010,
				COLOR_BLEND_EQUATION = 1000455011,
				COLOR_WRITE_MASK = 1000455012,
				TESSELLATION_DOMAIN_ORIGIN = 1000455002,
				RASTERIZATION_STREAM = 1000455013,
				CONSERVATIVE_RASTERIZATION_MODE = 1000455014,
				EXTRA_PRIMITIVE_OVERESTIMATION_SIZE = 1000455015,
				DEPTH_CLIP_ENABLE = 1000455016,
				SAMPLE_LOCATIONS_ENABLE = 1000455017,
				COLOR_BLEND_ADVANCED = 1000455018,
				PROVOKING_VERTEX_MODE = 1000455019,
				LINE_RASTERIZATION_MODE = 1000455020,
				LINE_STIPPLE_ENABLE = 1000455021,
				DEPTH_CLIP_NEGATIVE_ONE_TO_ONE = 1000455022,
				VIEWPORT_W_SCALING_ENABLE = 1000455023,
				VIEWPORT_SWIZZLE = 1000455024,
				COVERAGE_TO_COLOR_ENABLE = 1000455025,
				COVERAGE_TO_COLOR_LOCATION = 1000455026,
				COVERAGE_MODULATION_MODE = 1000455027,
				COVERAGE_MODULATION_TABLE_ENABLE = 1000455028,
				COVERAGE_MODULATION_TABLE = 1000455029,
				SHADING_RATE_IMAGE_ENABLE = 1000455030,
				REPRESENTATIVE_FRAGMENT_TEST_ENABLE = 1000455031,
				COVERAGE_REDUCTION_MODE = 1000455032,
				ATTACHMENT_FEEDBACK_LOOP_ENABLE = 1000524000,
				LINE_STIPPLE = 1000259000,
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
					// void setPushConstants();
					// void addPushConstant();
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

			void setFlags(const Flags& flags);

			void addVertexBinding(const VertexBinding& binding);
			void setVertexBindings(const std::vector<VertexBinding>& bindings);

			void addViewport(const Viewport& viewport);
			void setViewports(const std::vector<Viewport>& viewports);
			void addScissor(const Scissor& scissor);
			void setScissors(const std::vector<Scissor>& scissors);

			void setPrimitiveTopology(const Topology& topology);
			void enablePrimitiveRestart(const bool& enable = true);

			void enableDepthClamp(const bool& enable = true);
			void enableRasterizerDiscard(const bool& enable = true);
			void setPolygonMode(const PolygonMode& mode);
			void setCullMode(const CullMode& mode);
			void setFrontFace(const FrontFace& frontFace);
			void enableDepthBias(const bool& enable = true);
			void setDepthBiasConstant(const float& constant);
			void setDepthBiasSlope(const float& slope);
			void setDepthBiasClamp(const float& clamp);
			void setLinceWidth(const float& width);

			void setSampleCount(const SampleCount& count);
			void enableSampleShading(const bool& enable = true);
			void setMinimumSampleShading(const float& min);
			void enableAlphaToCoverage(const bool& enable = true);
			void enableAlphaToOne(const bool& enable = true);
			// TODO: sample mask

			void setColorBlendFlags(const ColorBlendFlags& flags);
			void enableBlendLogicOperation(const bool& enable = true);
			void setBlendLogicOperation(const LogicOp& op);
			void addColorAttachment(const ColorAttachment& attachment);
			void setColorAttachments(const std::vector<ColorAttachment>& attachments);
			void setBlendConstant(const Color::Components::Bits& component, const float& constant);

			void enableDepthTest(const bool& enable = true);
			void enableDepthWrite(const bool& enable = true);
			void setDepthCompareOp(const CompareOp& op);
			void enableDepthBoundsTest(const bool& enable);
			void enableStencilTest(const bool& enable);
			void setFrontStencilOpertions(const StencilOpState& operations);
			void setBackStencilOpertions(const StencilOpState& operations);
			void setMaxDepthBounds(const float& max);
			void setMinDepthBounds(const float& min);

			void addDynamicState(const DynamicState& state);
			void setDynamicStates(const std::vector<DynamicState>& states);

			void setTellesationPatchControlPoints(const std::size_t& count);

			bool isInitialized() const noexcept;
			void* getNativeHandle() const;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

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