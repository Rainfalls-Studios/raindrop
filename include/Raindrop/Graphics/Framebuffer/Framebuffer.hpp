#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/Framebuffer/common.hpp>
#include <Raindrop/Graphics/RenderPass/common.hpp>
#include <Raindrop/Graphics/Image/common.hpp>

namespace Raindrop::Graphics::Framebuffer{
	class Framebuffer{
		public:
			Framebuffer(Context& context, FramebufferBuilder& builder);
			~Framebuffer();

		private:
			Context& _context;
			VkFramebuffer _framebuffer;

			std::shared_ptr<RenderPass::RenderPass> _renderPass;
			std::vector<std::shared_ptr<Image::ImageView>> _attachments;

			uint32_t _width;
			uint32_t _height;
			uint32_t _layers;
	};
}

#endif