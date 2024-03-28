#ifndef __RAINDROP_RENDERER_BASE_FRAMEBUFFER_FRAMEBUFFER_HPP__
#define __RAINDROP_RENDERER_BASE_FRAMEBUFFER_FRAMEBUFFER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::BaseFramebuffer{
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

			Framebuffer(Context& context);
			Framebuffer(Context& context, glm::uvec2 size);
			~Framebuffer();

			const Attachment& depth() const;
			const Attachment& color() const;

			VkFramebuffer framebuffer() const;
			glm::uvec2 size() const;

			void resize(const glm::uvec2 size);
		
		private:

			std::unique_ptr<Attachment> _depthAttachment;
			std::unique_ptr<Attachment> _colorAttachment;

			Context& _context;

			VkFramebuffer _framebuffer;
			glm::uvec2 _size;

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