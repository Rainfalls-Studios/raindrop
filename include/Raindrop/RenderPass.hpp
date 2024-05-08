#ifndef __RAINDROP_RENDER_PASS_HPP__
#define __RAINDROP_RENDER_PASS_HPP__

#include "common.hpp"
#include "Format.hpp"
#include "Texture.hpp"

namespace Raindrop{
	class RenderPass{
		public:
			using Handle = std::size_t;

			class Subpass{
				public:
					class Flags : public Utils::FlagsTemplate<Flags> {
						public:
							using FlagsTemplate<Flags>::FlagsTemplate;

							enum Bits : Bitset {
								NONE = 0,
								PER_VIEW_ATTRIBUTES = 1 << 0,
								PER_VIEW_POSITION_X_ONLY = 1 << 1,
								FRAGMENT_REGION = 1 << 2,
								SHADER_RESOLVE = 1 << 3,
								RASTERIZATION_ORDER_ATTACHMENT_COLOR_ACCESS = 1 << 4,
								RASTERIZATION_ORDER_ATTACHMENT_DEPTH_ACCESS = 1 << 5,
								RASTERIZATION_ORDER_ATTACHMENT_STENCIL_ACCESS = 1 << 6,
								ENABLE_LEGACY_DITHERING = 1 << 7
							};
					};

					class AttachmentsInfo{
						public:
							AttachmentsInfo();
							AttachmentsInfo(const std::size_t& index);
							AttachmentsInfo(const std::initializer_list<std::size_t>& indices);

							const std::list<std::size_t>& getIndices() const noexcept;

						private:
							std::list<std::size_t> _indices;
					};

					class Dependency{
						public:
							// void setProvider(const Subpass& source, const Pipeline::Stage& stage, const Access& access);
							// void setReceiver(const Pipeline::Stage& stage, const Access& access);

					};

					const static Subpass EXTERNAL;

					Subpass();

					void setFlags(const Flags& flags);
					const Flags& getFlags() const noexcept;

					void setColorAttachments(const AttachmentsInfo& attachments);
					void setDepthAttachments(const AttachmentsInfo& attachments);
					void setResolveAttachments(const AttachmentsInfo& attachments);
					void setPreserveAttachments(const AttachmentsInfo& attachments);

					// Dependencies

					Dependency addDependency(const Subpass& other);

				private:

			};

			class Flags : public Utils::FlagsTemplate<Flags> {
				public:
					using FlagsTemplate<Flags>::FlagsTemplate;

					enum Bits{
						NONE = 0,
						VK_RENDER_PASS_CREATE_TRANSFORM = 2
					};
			};

			static RenderPass Create(Context& context);

			RenderPass(const RenderPass& other);
			RenderPass& operator=(const RenderPass& other);

			void initialize();
			void release();

			bool isInitialized() const noexcept;
			void* getNativeHandle() const noexcept;
			Handle getHandle() const noexcept;
			GUID getGUID() const noexcept;

			void addAttachment(const std::size_t& index, const Format& format, const Texture::Operation& operation, const Texture::Layout& initLayout, const Texture::Layout& finalLayout);
			Subpass addSubpass();

		private:
			Handle _handle;
	};

	static RenderPass CreateRenderPass(Context& context){
		return RenderPass::Create(context);
	}
}

#endif