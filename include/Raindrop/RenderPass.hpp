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

			class Access : public Utils::FlagsTemplate<Access> {
				public:
					using FlagsTemplate<Access>::FlagsTemplate;

					enum Bits : Bitset {
						NONE = 0,
						INDIRECT_COMMAND_READ = 0x00000001,
						INDEX_READ = 0x00000002,
						VERTEX_ATTRIBUTE_READ = 0x00000004,
						UNIFORM_READ = 0x00000008,
						INPUT_ATTACHMENT_READ = 0x00000010,
						SHADER_READ = 0x00000020,
						SHADER_WRITE = 0x00000040,
						COLOR_ATTACHMENT_READ = 0x00000080,
						COLOR_ATTACHMENT_WRITE = 0x00000100,
						DEPTH_STENCIL_ATTACHMENT_READ = 0x00000200,
						DEPTH_STENCIL_ATTACHMENT_WRITE = 0x00000400,
						TRANSFER_READ = 0x00000800,
						TRANSFER_WRITE = 0x00001000,
						HOST_READ = 0x00002000,
						HOST_WRITE = 0x00004000,
						MEMORY_READ = 0x00008000,
						MEMORY_WRITE = 0x00010000,
						TRANSFORM_FEEDBACK_WRITE = 0x02000000,
						TRANSFORM_FEEDBACK_COUNTER_READ = 0x04000000,
						TRANSFORM_FEEDBACK_COUNTER_WRITE = 0x08000000,
						CONDITIONAL_RENDERING_READ = 0x00100000,
						COLOR_ATTACHMENT_READ_NONCOHERENT = 0x00080000,
						ACCELERATION_STRUCTURE_READ = 0x00200000,
						ACCELERATION_STRUCTURE_WRITE = 0x00400000,
						FRAGMENT_DENSITY_MAP_READ = 0x01000000,
						FRAGMENT_SHADING_RATE_ATTACHMENT_READ = 0x00800000,
						COMMAND_PREPROCESS_READ = 0x00020000,
						COMMAND_PREPROCESS_WRITE = 0x00040000
					};
			};

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						CREATE_TRANSFORM = 2
					};
			};

			class Attachment{
				friend class RenderPass;
				public:
					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits{
								NONE = 0,
								MAY_ALIAS = 1
							};
					};

					class Operation{
						public:
							enum Load{
								LOAD,
								CLEAR,
								DONT_LOAD
							};

							enum Store{
								STORE,
								DONT_STORE
							};
					};

					Attachment& setFlags(const Flags& flags);
					Attachment& setFormat(const Format& format);
					// Attachment& setSamples(const )
					Attachment& setLoadOperation(const Operation::Load& operation);
					Attachment& setStoreOperation(const Operation::Store& operation);
					Attachment& setStencilLoadOperation(const Operation::Load& operation);
					Attachment& setStencilStoreOperation(const Operation::Store& operation);
					Attachment& setInitialLayout(const Texture::Layout& layout);
					Attachment& setFinalLayout(const Texture::Layout& layout);

				private:
					Attachment(RenderPass& owner, void* _data);
					void* _data;
					RenderPass& _owner;
			};

			class Subpass{
				friend class RenderPass;
				public:
					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits{
								NONE = 0,
								PER_VIEW_ATTRIBUTES = 0x00000001,
								PER_VIEW_POSITION_X_ONLY = 0x00000002,
								FRAGMENT_REGION = 0x00000004,
								SHADER_RESOLVE = 0x00000008,
								RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS = 0x00000010,
								RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS = 0x00000020,
								RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS = 0x00000040,
								ENABLE_LEGACY_DITHERING = 0x00000080,
							};
					};

					struct AttachmentReference{
						Attachment attachment;
						Texture::Layout layout;
					};

					Subpass& setFlags(const Flags& flags);
					// Subpass& setBindPoint(const Pipeline::BindPoint& bindPoint);
					Subpass& setDepthAttachment(const AttachmentReference& attachment);
					Subpass& addColorAttachment(const AttachmentReference& attachment);
					Subpass& addInputAttachment(const AttachmentReference& attachment);
					Subpass& addPreserveAttachment(const Attachment& attachment);

					static const Subpass External;

				private:
					Subpass(RenderPass* owner, void* data);
					void* _data;
					RenderPass* _owner;
			};

			class Dependency{
				friend class RenderPass;
				public:
					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits{
								NONE = 0,
								BY_REGION = 0x00000001,
								DEVICE_GROUP = 0x00000004,
								VIEW_LOCAL = 0x00000002,
								FEEDBACK_LOOP = 0x00000008,
							};
					};

					Dependency& setFlags(const Flags& flags);
					Dependency& setSrcSubpass(const Subpass& subpass);
					Dependency& setDstSubpass(const Subpass& subpass);
					Dependency& setSrcStage(const Pipeline::Stage& stage);
					Dependency& setDstStage(const Pipeline::Stage& stage);
					Dependency& setSrcAccess(const Access& access);
					Dependency& setDstAccess(const Access& access);

				private:
					Dependency(RenderPass& owner, void* data);
					void* _data;
					RenderPass& _owner;
			};

			static RenderPass Create(Context& context);
			
			RenderPass(Context& context);
			RenderPass(const RenderPass& other);
			RenderPass& operator=(const RenderPass& other);
			~RenderPass();

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const noexcept;
			Impl* getImpl() const noexcept;
			GUID getGUID() const noexcept;

			Attachment addAttachment();
			Subpass addSubpass();
			Dependency addDependency();

			// std::size_t addAttachment(const std::size_t& index, const Format& format, const Texture::Operation& operation, const Texture::Layout& initLayout, const Texture::Layout& finalLayout);
			// std::size_t addSubpass(const AttachmentsInfo& colorAttachments, const std::size_t& depth = SIZE_MAX, const AttachmentsInfo& resolveAttachments, const AttachmentsInfo& preserveAttachments);
			// void addDependency(const std::size_t& src, const std::size_t& dst, const Pipeline::Stage& srcStage, const Access& srcAccess, const Pipeline::Stage& dstStage, const Access& dstAccess);

		private:
			friend class Subpass;
			friend class Attachment;
			Impl* _impl;
	};

	static RenderPass CreateRenderPass(Context& context){
		return RenderPass::Create(context);
	}
}

#endif