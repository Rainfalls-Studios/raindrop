#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/ForwardShader/common.hpp>

namespace Raindrop::Graphics::ForwardShader{
	class Framebuffer{
		public:
			Framebuffer(GraphicsContext& context, uint32_t width, uint32_t height);
			~Framebuffer();

			uint32_t width() const;
			uint32_t height() const;

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			VkDescriptorImageInfo attachmentInfo() const;

		private:
			GraphicsContext& _context;
			
			VkDeviceMemory _memory;
			VkImage _image;
			VkImageView _imageView;
			VkSampler _sampler;
			VkFramebuffer _framebuffer;

			uint32_t _width;
			uint32_t _height;

			void createImage();
			void allocateMemory();
			void bindImageMemory();
			void createImageView();
			void createSampler();
			void createFramebuffer();

			void transitionImage(VkCommandBuffer commandBuffer);
	};
}

#endif