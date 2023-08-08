#ifndef __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_SHADOW_MAP_HPP__
#define __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_SHADOW_MAP_HPP__

#include <Raindrop/Graphics/ShadowMap/Sun/common.hpp>

namespace Raindrop::Graphics::ShadowMap::Sun{
	class ShadowMap{
		public:
			ShadowMap(GraphicsContext& context, uint32_t width = 1024, uint32_t height = 1024);
			~ShadowMap();

			void beginRenderPass(VkCommandBuffer commandBuffer) const;
			void endRenderPass(VkCommandBuffer commandBuffer) const;

			VkDescriptorImageInfo getAttachmentInfo() const;

			uint32_t width() const;
			uint32_t height() const;

		private:
			GraphicsContext& _context;

			uint32_t _width;
			uint32_t _height;

			void createAttachment();
			void createFramebuffer();

			void createImage();
			void allocateMemory();
			void createImageView();
			void createSampler();
			
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _imageView = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;

			VkFramebuffer _framebuffer = VK_NULL_HANDLE;
	};
}

#endif