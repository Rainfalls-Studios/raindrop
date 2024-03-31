#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_RENDER_PASS_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	class RenderPass{
		public:
			RenderPass(Context& context);
			~RenderPass();

			VkRenderPass get() const;

		private:
			Context& _context;

			VkRenderPass _renderPass;

			void createRenderPass();
	};
}

#endif