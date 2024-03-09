#ifndef __RAINDROP_RENDERER_SCENE_FRAMEBUFFER_HPP__
#define __RAINDROP_RENDERER_SCENE_FRAMEBUFFER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Scene{
	class Framebuffer{
		public:
			Framebuffer(Context& context);
			Framebuffer(Context& context, glm::uvec2 size);
			~Framebuffer();
		
		private:
			struct Attachment{
				VkImageView imageView;	
				VkImage image;
			};

			Attachment _depthAttachment;
			Attachment _colorAttachment;

			Context& _context;

			VkFramebuffer _framebuffer;
			glm::uvec2 _size;

			void createAttachments();
			void destroyAttachments();
			void createDepthAttachment();
			void createColorAttachment();
			VkImageCreateInfo depthAttachmentInfo(); 
			VkImageCreateInfo colorAttachmentInfo(); 


			void createFramebuffer();


	};
}

#endif