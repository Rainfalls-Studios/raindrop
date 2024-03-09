#ifndef __RAINDROP_RENDERER_SCENE_RENDER_PASS_HPP__
#define __RAINDROP_RENDERER_SCENE_RENDER_PASS_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Scene{
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