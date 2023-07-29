#ifndef __RAINDROP_GRAPHICS_SCENE_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_SCENE_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	struct PushConstant{
		glm::mat4 viewTransform;
		glm::mat4 localTransform;
	};
	class SceneRenderer{
		public:
			SceneRenderer(GraphicsContext& context);
			~SceneRenderer();
			
			void drawEntity(Core::Scene::Entity entity, VkPipelineLayout layout, VkCommandBuffer commandBuffer, glm::mat4& viewTransform);
			
		private:
			GraphicsContext& _context;

			std::shared_ptr<DescriptorPool> _descriptorPool;
			std::shared_ptr<DescriptorSetLayout> _setLayout;
			VkDescriptorSet _descriptorSet;

			
			void createDescriptorPool();
			void createSetLayout();
			void createDescriptorSet();
	};
}

#endif