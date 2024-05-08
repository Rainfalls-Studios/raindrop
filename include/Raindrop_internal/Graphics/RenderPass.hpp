#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__

#include "common.hpp"

namespace Raindrop::Internal::Graphics{
	struct RenderPassConfigInfo{
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkSubpassDescription> subpasses;
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