#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_FORWARD_SHADER_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_FORWARD_SHADER_HPP__

#include <Raindrop/Graphics/ForwardShader/common.hpp>
#include <Raindrop/Graphics/ForwardShader/Pipelines/common.hpp>

namespace Raindrop::Graphics::ForwardShader{
	class ForwardShader{
		public:
			ForwardShader(GraphicsContext& context);
			~ForwardShader();

			void render(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection);
			void updateDescriptor(WorldFramebuffer &worldFramebuffer);

			VkDescriptorImageInfo attachmentInfo() const;

		private:
			GraphicsContext& _context;

			std::unique_ptr<DescriptorSet> _descriptorSet;
			std::unique_ptr<Framebuffer> _framebuffer;
			std::unique_ptr<Pipelines::Pipelines> _pipelines;

			void renderLightPoints(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection);
			void renderSpotlights(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& cameraDirection);
			void renderSuns(VkCommandBuffer commandBuffer, const glm::vec3& cameraPosition, const glm::vec3& );

	};
}

#endif