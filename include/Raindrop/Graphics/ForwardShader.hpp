#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class ForwardShader : public Target{
		public:
			ForwardShader(GraphicsContext& context, uint32_t width, uint32_t height);
			~ForwardShader();

			virtual VkRenderPass renderPass() const override;
			virtual uint32_t attachmentCount() const override;

			void render(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition);

			void resize(uint32_t width, uint32_t height);
			void updateDescriptor(WorldFramebuffer& worldFramebuffer);

			VkDescriptorImageInfo getAttachmentInfo() const;

		private:
			GraphicsContext& _context;

			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _imageView = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;
			
			VkFramebuffer _framebuffer;
			VkRenderPass _renderPass;
			
			std::shared_ptr<GraphicsPipeline> _lightPointPipeline;
			std::shared_ptr<GraphicsPipeline> _spotlightPipeline;
			std::shared_ptr<GraphicsPipeline> _sunPipeline;

			std::shared_ptr<DescriptorPool> _descriptorPool;
			std::shared_ptr<DescriptorSetLayout> _setLayout;
			VkDescriptorSet _descriptorSet;

			uint32_t _width;
			uint32_t _height;

			void createRenderPass();
			void createAttachment();
			void createFramebuffer();
			void createImage();
			void createImageView();
			void createSampler();
			void checkDefaultFormats();
			
			void createDescriptorPool();
			void createSetLayout();
			void createDescriptorSet();

			void createLightPointPipeline();
			void createSpotlightPipeline();
			void createSunPipeline();

			void renderLightPoints(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition);
			void renderSpotlights(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition);
			void renderSuns(VkCommandBuffer commandBuffer, const glm::vec3& cameraDirection, const glm::vec3& cameraPosition);

			void destroyResizable();

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);
	};
}

#endif