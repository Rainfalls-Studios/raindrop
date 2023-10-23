#ifndef __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_EDITOR_VIEWPORT_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/Editor/common.hpp>
#include <Raindrop/Graphics/Editor/Image.hpp>

namespace Raindrop::Graphics::Editor{
	class ViewportFramebuffer : public Image{
		public:
			ViewportFramebuffer(EditorContext& context, uint32_t width, uint32_t height);
			~ViewportFramebuffer();

			bool beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			virtual uint32_t width() const override;
			virtual uint32_t height() const override;

			VkRenderPass renderPass() const;

		private:
			EditorContext& _context;
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