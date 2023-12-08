#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_BUILDER_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_RENDER_PASS_BUILDER_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::RenderPass{
	class RenderPassBuilder{
		public:
			RenderPassBuilder(Context& context);
			~RenderPassBuilder();

			void loadFromNode(const YAML::Node& node);

			const VkAttachmentDescription* attachments() const;
			uint32_t attachmentCount() const;

			const VkSubpassDescription* subpasses() const;
			uint32_t subpassCount() const;

			const VkSubpassDependency* dependencies() const;
			uint32_t dependencyCount() const;

			VkRenderPassCreateFlags flags() const;
			const std::string& name() const;

		private:
			Context& _context;

			struct SubpassData{
				std::vector<uint32_t> preserveAttachments;
				std::vector<VkAttachmentReference> inputAttachments;
				std::vector<VkAttachmentReference> resolveAttachments;
				std::vector<VkAttachmentReference> colorAttachments;
				VkAttachmentReference depthAttachment;
			};

			std::vector<VkAttachmentDescription> _attachmentDescriptions;
			std::vector<VkSubpassDescription> _subpasses;
			std::vector<SubpassData> _subpassDatas;
			std::vector<VkSubpassDependency> _dependencies;
			VkRenderPassCreateFlags _flags;
			std::string _name;

			std::unordered_map<std::string, std::size_t> _attachmentToId;
			std::unordered_map<std::string, std::size_t> _subpassToId;

			void loadAttachment(const YAML::Node& node);
			void loadSubpass(const YAML::Node& node);
			void loadDependency(const YAML::Node& node);

			VkAttachmentReference loadAttachmentReference(const YAML::Node& node);
			void loadAttachmentRef(std::vector<VkAttachmentReference>& storage, const YAML::Node& node);
			void loadPreserveAttachments(std::vector<uint32_t>& storage, const YAML::Node& node);

			uint32_t findSubpass(const YAML::Node& node);
	};
}

#endif