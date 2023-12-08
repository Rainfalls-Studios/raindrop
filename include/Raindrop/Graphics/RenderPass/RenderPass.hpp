#ifndef __RAINDROP_GRAPHICS_RENDER_PASS_HPP__
#define __RAINDROP_GRAPHICS_RENDER_PASS_HPP__

#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::RenderPass{
	class RenderPass{
		public:
			RenderPass(Context& context, const RenderPassBuilder& builder);
			~RenderPass();

			VkRenderPass get() const;
		
		private:
			Context& _context;

			VkRenderPass _renderPass;
	};
}

#endif