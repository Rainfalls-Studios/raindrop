#ifndef __RAINDROP_RENDERER_BASE_FRAMEBUFFER_BASE_FRAMEBUFFER_HPP__
#define __RAINDROP_RENDERER_BASE_FRAMEBUFFER_BASE_FRAMEBUFFER_HPP__

#include "common.hpp"
#include "Context.hpp"

namespace Raindrop::Renderer::BaseFramebuffer{
	class BaseFramebuffer{
		public:
			BaseFramebuffer(::Raindrop::Renderer::Context& renderer);
			~BaseFramebuffer();
			
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