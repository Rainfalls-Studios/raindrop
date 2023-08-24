#ifndef __RAINDROP_GRAPHICS_SCENE_FRAMEBUFFER_HPP__
#define __RAINDROP_GRAPHICS_SCENE_FRAMEBUFFER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class SceneFramebuffer : public Target{
		public:
			struct Attachment{
				VkDeviceMemory memory = VK_NULL_HANDLE;
				VkImage image = VK_NULL_HANDLE;
				VkImageView imageView = VK_NULL_HANDLE;
				VkSampler sampler = VK_NULL_HANDLE;
			};

			SceneFramebuffer(GraphicsContext& context, uint32_t width, uint32_t height);
			~SceneFramebuffer();

			virtual VkRenderPass renderPass() const override;
			virtual uint32_t attachmentCount() const override;
			virtual float aspectRatio() const override;
			virtual uint32_t width() const override;
			virtual uint32_t height() const override;

			virtual bool beginRenderPass(VkCommandBuffer commandBuffer);
			virtual void endRenderPass(VkCommandBuffer commandBuffer);

			Attachment& getAttachment(uint32_t id);
			std::vector<Attachment>& getAttachments();

			virtual VkDescriptorImageInfo getAttachmentInfo(uint32_t attachment) const override;
			virtual VkDescriptorSet descriptorSet() const override;

			void resize(uint32_t width, uint32_t height);

		private:
			GraphicsContext& _context;

			VkFramebuffer _framebuffer;
			std::vector<Attachment> _attachments;
			VkDescriptorSet _descriptorSet;

			uint32_t _width;
			uint32_t _height;
			
			void createAttachments();
			void createFramebuffer();
			void checkDefaultFormats();
			void allocDescriptorSet();
			void updateDescriptorSet();
			void freeDescriptorSet();
	};
}

#endif