#ifndef __RAINDROP_GRAPHICS_SCENE_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_SCENE_RENDERER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Core/Scene/Entity.hpp>

namespace Raindrop::Graphics{
	struct PushConstant{
		glm::mat4 viewTransform;
		glm::mat4 localTransform;
	};

	class SceneRenderer{
		public:
			SceneRenderer(GraphicsContext& context);
			~SceneRenderer();

			struct RenderWithTextureInfo{
				VkPipelineLayout layout;
				VkCommandBuffer commandBuffer;
				glm::mat4 viewTransform;
			};
			
			void renderWithTextures(Core::Scene::Entity entity, const RenderWithTextureInfo& info);

			struct RenderForDepthMapInfo{
				Core::Scene::Entity entity;
				VkPipelineLayout layout;
				VkCommandBuffer commandBuffer;
				glm::mat4 viewTransform;
			};

			void renderForDepthMap(Core::Scene::Entity entity, const RenderForDepthMapInfo& info);
			
		private:
			GraphicsContext& _context;
	};
}

#endif