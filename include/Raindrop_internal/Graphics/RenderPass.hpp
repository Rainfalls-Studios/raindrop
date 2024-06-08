#ifndef __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_RENDER_PASS_HPP__

#include "common.hpp"
#include <optional>

namespace Raindrop::Internal::Graphics{
	using RenderPassConfigInfo = VkRenderPassCreateInfo;

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