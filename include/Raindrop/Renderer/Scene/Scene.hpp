#ifndef __RAINDROP_RENDERER_SCENE_SCENE_HPP__
#define __RAINDROP_RENDERER_SCENE_SCENE_HPP__

#include "common.hpp"
#include "Context.hpp"

namespace Raindrop::Renderer::Scene{
	class Scene{
		public:
			Scene(::Raindrop::Renderer::Context& renderer);
			~Scene();
			
			void resize(const glm::uvec2& size);


			void swapchainRender(VkCommandBuffer commandBuffer);

			void beginRenderPass(VkCommandBuffer commandBuffer);
			void endRenderPass(VkCommandBuffer commandBuffer);

			VkRenderPass renderPass() const;

		private:
			::Raindrop::Renderer::Context& _renderer;
			Context _context;

			VkClearValue clearColor;
	};
}

#endif