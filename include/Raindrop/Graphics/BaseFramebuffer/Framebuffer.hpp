#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_FRAMEBUFFER_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	class Framebuffer{
		public:
			struct Attachment{
				Context& context;

				VkImageView imageView;	
				VkImage image;
				VkDeviceMemory memory;

				void create(const VkImageCreateInfo& imageInfo, VkImageViewCreateInfo imageViewInfo);

				Attachment(Context& context);
				~Attachment();
			};

			Framebuffer(::Raindrop::Graphics::Context& renderer);
			Framebuffer(::Raindrop::Graphics::Context& renderer, glm::uvec2 size);
			~Framebuffer();
			
			void swapchainRender(VkCommandBuffer commandBuffer);

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);


			VkRenderPass renderPass() const;

			const Attachment& depth() const;
			const Attachment& color() const;

			VkFramebuffer framebuffer() const;
			glm::uvec2 size() const;

			void resize(const glm::uvec2& size);
		
		private:
			::Raindrop::Graphics::Context& _renderer;
			Context* _context;

			std::unique_ptr<Attachment> _depthAttachment;
			std::unique_ptr<Attachment> _colorAttachment;

			VkFramebuffer _framebuffer;
			glm::uvec2 _size;

			VkClearValue clearColor;

			void createAttachments();
			void destroyAttachments();
			void createDepthAttachment();
			void createColorAttachment();

			VkImageCreateInfo depthImageInfo(); 
			VkImageViewCreateInfo depthImageViewInfo();

			VkImageCreateInfo colorImageInfo(); 
			VkImageViewCreateInfo colorImageViewInfo(); 

			void createFramebuffer();

			void reset();


	};
}

#endif