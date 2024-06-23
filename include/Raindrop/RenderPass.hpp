#ifndef __RAINDROP_RENDER_PASS_HPP__
#define __RAINDROP_RENDER_PASS_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "Texture.hpp"
#include "Pipeline.hpp"

namespace Raindrop{
	class RenderPass{
		public:
			struct Impl;

			enum class AccessBits{
				NONE = 0,
				INDIRECT_COMMAND_READ = 1 << 0,
				INDEX_READ = 1 << 1,
				VERTEX_ATTRIBUTE_READ = 1 << 2,
				UNIFORM_READ = 1 << 3,
				INPUT_ATTACHMENT_READ = 1 << 4,
				SHADER_READ = 1 << 5,
				SHADER_WRITE = 1 << 6,
				COLOR_ATTACHMENT_READ = 1 << 7,
				COLOR_ATTACHMENT_WRITE = 1 << 8,
				DEPTH_STENCIL_ATTACHMENT_READ = 1 << 9,
				DEPTH_STENCIL_ATTACHMENT_WRITE = 1 << 10,
				TRANSFER_READ = 1 << 11,
				TRANSFER_WRITE = 1 << 12,
				HOST_READ = 1 << 13,
				HOST_WRITE = 1 << 14,
				MEMORY_READ = 1 << 15,
				MEMORY_WRITE = 1 << 16,
				TRANSFORM_FEEDBACK_WRITE = 1 << 17,
				TRANSFORM_FEEDBACK_COUNTER_READ = 1 << 18,
				TRANSFORM_FEEDBACK_COUNTER_WRITE = 1 << 19,
				CONDITIONAL_RENDERING_READ = 1 << 20,
				COLOR_ATTACHMENT_READ_NONCOHERENT = 1 << 21,
				ACCELERATION_STRUCTURE_READ = 1 << 22,
				ACCELERATION_STRUCTURE_WRITE = 1 << 23,
				FRAGMENT_DENSITY_MAP_READ = 1 << 24,
				FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 1 << 25,
				COMMAND_PREPROCESS_READ = 1 << 26,
				COMMAND_PREPROCESS_WRITE = 1 << 27
			};
			RAINDROP_FLAGS_CLASS(Access, AccessBits);

			enum class FlagsBits{
				NONE = 0,
			};
			RAINDROP_FLAGS_CLASS(Flags, FlagsBits);

			class AttachmentDescription{
				friend class RenderPass;
				public:
					enum class FlagsBits{
						NONE = 0,
						MAY_ALIAS = 1 << 0
					};
					RAINDROP_FLAGS_CLASS(Flags, FlagsBits);

					class Operation{
						public:
							enum Load{
								LOAD,
								CLEAR,
								DONT_LOAD,
								LOAD_NONE,
							};

							enum Store{
								STORE,
								DONT_STORE,
								STORE_NONE,
							};
					};

					// AttachmentDescription& setSamples(const )

					AttachmentDescription() noexcept;

					AttachmentDescription(const AttachmentDescription&) = delete;
					AttachmentDescription& operator=(const AttachmentDescription&) = delete;

					AttachmentDescription(AttachmentDescription&&) = default;
					AttachmentDescription& operator=(AttachmentDescription&&) = default;

					AttachmentDescription& setFlags(const Flags& flags) noexcept;
					AttachmentDescription& setFormat(const Format& format) noexcept;
					AttachmentDescription& setLoadOperation(const Operation::Load& operation) noexcept;
					AttachmentDescription& setStoreOperation(const Operation::Store& operation) noexcept;
					AttachmentDescription& setStencilLoadOperation(const Operation::Load& operation) noexcept;
					AttachmentDescription& setStencilStoreOperation(const Operation::Store& operation) noexcept;
					AttachmentDescription& setInitialLayout(const Texture::Layout& layout) noexcept;
					AttachmentDescription& setFinalLayout(const Texture::Layout& layout) noexcept;

					const Flags& getFlags() const noexcept;
					const Format& getFormat() const noexcept;
					const Operation::Load& getLoadOperation() const noexcept;
					const Operation::Store& getStoreOperation() const noexcept;
					const Operation::Load& getStencilLoadOperation() const noexcept;
					const Operation::Store& getStencilStoreOperation() const noexcept;
					const Texture::Layout& getInitialLayout() const noexcept;
					const Texture::Layout& getFinalLayout() const noexcept;

				private:
					Flags _flags;
					Format _format;
					Operation::Load _loadOperation;
					Operation::Store _storeOperation;
					Operation::Load _stencilLoadOperation;
					Operation::Store _stencilStoreOperation;
					Texture::Layout _initialLayout;
					Texture::Layout _finalLayout;
			};

			class Subpass{
				friend class RenderPass;
				public:
					enum class FlagsBits{
						NONE = 0,
						PER_VIEW_ATTRIBUTES = 1 << 0,
						PER_VIEW_POSITION_X_ONLY = 1 << 1,
						FRAGMENT_REGION = 1 << 2,
						SHADER_RESOLVE = 1 << 3,
						RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS = 1 << 4,
						RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS = 1 << 5,
						RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS = 1 << 6,
						ENABLE_LEGACY_DITHERING = 1 << 7,
					};
					RAINDROP_FLAGS_CLASS(Flags, FlagsBits);

					struct AttachmentReference{
						AttachmentDescription& attachment;
						Texture::Layout layout;
					};

					// Subpass& setBindPoint(const Pipeline::BindPoint& bindPoint);

					Subpass();

					Subpass(const Subpass&) = delete;
					Subpass& operator=(const Subpass&) = delete;

					Subpass(Subpass&&) = default;
					Subpass& operator=(Subpass&&) = default;

					Subpass& setFlags(const Flags& flags) noexcept;
					Subpass& setDepthAttachment(const AttachmentReference& attachment);
					Subpass& addColorAttachment(const AttachmentReference& attachment);
					Subpass& addInputAttachment(const AttachmentReference& attachment);
					Subpass& addPreserveAttachment(const AttachmentDescription& attachment);

					const Flags& getFlags() const noexcept;
					const std::optional<AttachmentReference>& getDepthAttachment() const noexcept;
					const std::list<AttachmentReference>& getColorAttachments() const noexcept;
					const std::list<AttachmentReference>& getInputAttachments() const noexcept;
					const std::list<std::reference_wrapper<const AttachmentDescription>>& getPreserveAttachments() const noexcept;

					static const Subpass External;

				private:
					Flags _flags;
					std::optional<AttachmentReference> _depthAttachment;
					std::list<AttachmentReference> _colorAttachments;
					std::list<AttachmentReference> _inputAttachments;
					std::list<std::reference_wrapper<const AttachmentDescription>> _preserveAttachments;
			};

			class Dependency{
				friend class RenderPass;
				public:
					enum class FlagsBits{
						NONE = 0,
						BY_REGION = 1 << 0,
						DEVICE_GROUP = 1 << 1,
						VIEW_LOCAL = 1 << 2,
						FEEDBACK_LOOP = 1 << 3,
					};
					RAINDROP_FLAGS_CLASS(Flags, FlagsBits);

					Dependency();

					Dependency(const Dependency&) = delete;
					Dependency& operator=(const Dependency&) = delete;

					Dependency(Dependency&&) = default;
					Dependency& operator=(Dependency&&) = default;

					Dependency& setFlags(const Flags& flags);
					Dependency& setSrcSubpass(const Subpass& subpass);
					Dependency& setDstSubpass(const Subpass& subpass);
					Dependency& setSrcStage(const Pipeline::Stage& stage);
					Dependency& setDstStage(const Pipeline::Stage& stage);
					Dependency& setSrcAccess(const Access& access);
					Dependency& setDstAccess(const Access& access);

					const Flags& getFlags() const noexcept;
					const Subpass& getSrcSubpass() const noexcept;
					const Subpass& getDstSubpass() const noexcept;
					const Pipeline::Stage& getSrcStage() const noexcept;
					const Pipeline::Stage& getDstStage() const noexcept;
					const Access& getSrcAccess() const noexcept;
					const Access& getDstAccess() const noexcept;

				private:
					Flags _flags;
					const Subpass* _srcSubpass;
					const Subpass* _dstSubpass;
					Pipeline::Stage _srcStage;
					Pipeline::Stage _dstStage;
					Access _srcAccess;
					Access _dstAccess;
			};

			static RenderPass Create(Context& context);
			
			RenderPass(Context& context);
			~RenderPass();

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const noexcept;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			AttachmentDescription& addAttachment();
			Subpass& addSubpass();
			Dependency& addDependency();

			void begin(CommandBuffer& commandBuffer);

		private:
			std::unique_ptr<Impl> _impl;
	};

	static RenderPass CreateRenderPass(Context& context){
		return RenderPass::Create(context);
	}
}

#endif