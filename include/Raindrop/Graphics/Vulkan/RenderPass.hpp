#pragma once

#include "Raindrop/pch.pch"
#include "Raindrop/Core/Object.hpp"

namespace Raindrop::Graphics{
	class Context;

	namespace Vulkan{
		class RenderPass : public Core::Object{
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

				class Builder{
					friend class RenderPass;
					public:
						Builder(Context& context);
						~Builder();

						void setCreateFlags(const VkRenderPassCreateFlags& flags);

						AttachmentDescription addAttachment();
						SubpassDescription addSubpass();
						Dependency addDependency();

					private:
						Context& _context;
						BuildInfo _info;
				};

				static std::shared_ptr<RenderPass> Create(const Builder& builder);

				RenderPass(const Builder& builder);
				RenderPass(Context& context, const VkRenderPassCreateInfo& info);
				~RenderPass();

				RenderPass(const RenderPass&) = delete;
				RenderPass& operator=(const RenderPass&) = delete;

				const VkRenderPass& get() const noexcept;

			private:
				Context& _context;
				VkRenderPass _renderPass;
		};
	}
}