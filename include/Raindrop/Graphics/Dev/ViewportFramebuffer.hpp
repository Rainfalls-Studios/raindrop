#ifndef __RAINDROP_GRAPHICS_DEV_VIEWPORT_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_DEV_VIEWPORT_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/Dev/common.hpp>

namespace Raindrop::Graphics::Dev{
	class ViewportFramebuffer{
		public:
			ViewportFramebuffer(DevContext& context, uint32_t width, uint32_t height);
			~ViewportFramebuffer();

		private:
			DevContext& _context;
			VkFramebuffer _framebuffer = VK_NULL_HANDLE;
			VkRenderPass _renderPass = VK_NULL_HANDLE;

			VkImage _attachmentImage = VK_NULL_HANDLE;
			VkImageView _attachmentImageView = VK_NULL_HANDLE;
			VkDeviceMemory _attachmentMemory = VK_NULL_HANDLE;
			VkSampler _attachmentSampler = VK_NULL_HANDLE;

			uint32_t _width;
			uint32_t _height;

			void createFramebuffer();
			void createRenderPass();
			
			void createAttachmentImage();
			void createAttachmentImageView();
			void allocAttachmentMemory();
			void createAttachmentSampler();
	};
}

#endif