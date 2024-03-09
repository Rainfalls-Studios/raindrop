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
				Context& context;

				VkImageView imageView;	
				VkImage image;
				VkDeviceMemory memory;

				void create(const VkImageCreateInfo& imageInfo, VkImageViewCreateInfo imageViewInfo);

				Attachment(Context& context);
				~Attachment();
			};

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


	};
}

#endif