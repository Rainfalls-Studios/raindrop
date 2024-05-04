#ifndef __RAINDROP_GRAPHICS_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_FRAMEBUFFER_HPP__

#include "common.hpp"

namespace Raindrop::Graphics{
	struct FramebufferConfigInfo{
		std::shared_ptr<RenderPass> renderPass;
		std::vector<std::shared_ptr<ImageView>> attachments;
		uint32_t width;
		uint32_t height;
		uint32_t layers;
	};

	class Framebuffer{
		public:
			Framebuffer(Context& context, FramebufferConfigInfo& confuig);
			~Framebuffer();

			const VkFramebuffer& get() const;
			const std::shared_ptr<RenderPass>& renderPass() const;
			const std::vector<std::shared_ptr<ImageView>>& attachments() const;

		private:
			Context& _context;
			VkFramebuffer _framebuffer;

			std::shared_ptr<RenderPass> _renderPass;
			std::vector<std::shared_ptr<ImageView>> _attachments;

			uint32_t _width;
			uint32_t _height;
			uint32_t _layers;
	};
}

#endif