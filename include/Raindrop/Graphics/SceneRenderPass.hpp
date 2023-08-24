#ifndef __RAINDROP_GRAPHICS_SCENE_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_SCENE_RENDER_PASS_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class SceneRenderPass{
		public:
			SceneRenderPass(GraphicsContext& context);
			~SceneRenderPass();

			VkRenderPass get() const;

		private:
			GraphicsContext& _context;
			VkRenderPass _renderPass;

			void createRenderPass();
	};
}

#endif