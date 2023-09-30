#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/builders/common.hpp>

namespace Raindrop::Graphics{
	class Framebuffer{
		public:
			Framebuffer(GraphicsContext& context, const Builders::FramebufferBuilder& builder);
			~Framebuffer();

			VkFramebuffer get() const;
			std::shared_ptr<RenderPass> renderPass() const;

			uint32_t width() const;
			uint32_t height() const;
			uint32_t layers() const;

		private:
			GraphicsContext& _context;
			VkFramebuffer _framebuffer;

			uint32_t _width;
			uint32_t _height;
			uint32_t _layers;
			
			std::shared_ptr<RenderPass> _renderPass;
			std::vector<std::shared_ptr<ImageView>> _attachments; 

	};
}

#endif 