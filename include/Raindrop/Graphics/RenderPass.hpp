#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_HPP__

#include "prototypes.hpp"
#include <Raindrop/Graphics/Core/prototypes.hpp>
#include <Raindrop/Graphics/pch.pch>

namespace Raindrop::Graphics{
	class RenderPass{
		private:
			struct BuildInfo{
				struct SubpassData{
					VkAttachmentReference depth;
					std::vector<VkAttachmentReference> colorAttachments;
					std::vector<VkAttachmentReference> inputAttachments;
					std::vector<std::uint32_t> preserveAttachments;
				};

				VkRenderPassCreateFlags flags;
				std::vector<VkAttachmentDescription> attachmentDescriptions;
				std::vector<VkSubpassDescription> subpassDescriptions;
				std::vector<VkSubpassDependency> subpassDependencies;
				std::vector<SubpassData> subpassData;

				VkRenderPassCreateInfo generateInfo() const;
			};

		public:
			class AttachmentDescription{
				public:
					AttachmentDescription(VkAttachmentDescription& description, const std::uint32_t& index) noexcept;

					AttachmentDescription& setFlags(const VkAttachmentDescriptionFlags& flags) noexcept;
					AttachmentDescription& setFormat(const VkFormat& format) noexcept;
					AttachmentDescription& setLoadOperation(const VkAttachmentLoadOp& operation) noexcept;
					AttachmentDescription& setStoreOperation(const VkAttachmentStoreOp& operation) noexcept;
					AttachmentDescription& setStencilLoadOperation(const VkAttachmentLoadOp& operation) noexcept;
					AttachmentDescription& setStencilStoreOperation(const VkAttachmentStoreOp& operation) noexcept;
					AttachmentDescription& setInitialLayout(const VkImageLayout& layout) noexcept;
					AttachmentDescription& setFinalLayout(const VkImageLayout& layout) noexcept;

					VkAttachmentDescription& get() noexcept;
					const std::uint32_t& getIndex() const noexcept;

				private:
					VkAttachmentDescription* _description;
					std::uint32_t _index;
			};

			class SubpassDescription{
				friend class RenderPass;
				public:

					static SubpassDescription External;

					SubpassDescription(VkSubpassDescription& description, BuildInfo::SubpassData& data, const std::uint32_t& index) noexcept;
					
					SubpassDescription& setFlags(const VkSubpassDescriptionFlags& flags) noexcept;
					SubpassDescription& setDepthAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
					SubpassDescription& addColorAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
					SubpassDescription& addInputAttachment(const AttachmentDescription& attachment, VkImageLayout layout);
					SubpassDescription& addPreserveAttachment(const AttachmentDescription& attachment);

					VkSubpassDescription& get() noexcept;
					const std::uint32_t& getIndex() const noexcept;

				private:
					SubpassDescription();

					VkSubpassDescription* _description;
					BuildInfo::SubpassData* _data;
					std::uint32_t _index;
			};

			class Dependency{
				friend class RenderPass;
				public:
					Dependency(VkSubpassDependency& dependency) noexcept;

					Dependency& setFlags(const VkDependencyFlags& flags) noexcept;
					Dependency& setSrcSubpass(const SubpassDescription& subpass) noexcept;
					Dependency& setDstSubpass(const SubpassDescription& subpass) noexcept;
					Dependency& setSrcStage(const VkPipelineStageFlags& stage) noexcept;
					Dependency& setDstStage(const VkPipelineStageFlags& stage) noexcept;
					Dependency& setSrcAccess(const VkAccessFlags& access) noexcept;
					Dependency& setDstAccess(const VkAccessFlags& access) noexcept;

					VkSubpassDependency& get() noexcept;

				private:
					VkSubpassDependency* _dependency;
			};

			RenderPass() noexcept;
			~RenderPass();

			RenderPass(RenderPass&& other);
			RenderPass& operator=(RenderPass&& other);

			void prepare(Context& context);
			void initialize();
			void release();

			void setCreateFlags(const VkRenderPassCreateFlags& flags);

			AttachmentDescription addAttachment();
			SubpassDescription addSubpass();
			Dependency addDependency();

			const VkRenderPass& get() const noexcept;

			friend void swap(RenderPass& A, RenderPass& B);

		private:
			Context* _context;
			VkRenderPass _renderPass;

			std::unique_ptr<BuildInfo> _buildInfo;
	};
}

#endif