#ifndef __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_RENDER_PASS_HPP__

#include <Raindrop/Graphics/ShadowMap/Sun/common.hpp>

namespace Raindrop::Graphics::ShadowMap::Sun{
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