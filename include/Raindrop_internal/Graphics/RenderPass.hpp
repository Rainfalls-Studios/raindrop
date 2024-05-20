#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__

#include "common.hpp"
#include <optional>

namespace Raindrop::Internal::Graphics{
	struct RenderPassConfigInfo{
		std::vector<VkAttachmentDescription> attachments;

		struct SubpassInfo{
			VkSubpassDescription description;
			std::optional<VkAttachmentReference> depth;
			std::vector<VkAttachmentReference> color;
			std::vector<uint32_t> preserve;
			std::vector<VkAttachmentReference> input;
		};

		std::vector<SubpassInfo> subpasses;
		std::vector<VkSubpassDependency> dependencies;
		VkRenderPassCreateFlags flags;
	};

	class RenderPass{
		public:
			RenderPass(Context& context, const RenderPassConfigInfo& config);
			~RenderPass();

			VkRenderPass get() const;

		private:
			Context& _context;
			VkRenderPass _renderPass;

	};
}

#endif