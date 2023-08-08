#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_RENDER_PASS_HPP__

#include <Raindrop/Graphics/ForwardShader/common.hpp>

namespace Raindrop::Graphics::ForwardShader{
	class RenderPass{
		public:
			RenderPass(GraphicsContext& context);
			~RenderPass();

			VkRenderPass get() const;

		private:
			GraphicsContext& _context;
			VkRenderPass _renderPass;

			void createRenderPass();
	};
}

#endif