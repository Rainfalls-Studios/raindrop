#ifndef __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_FULLSCREEN_QUAD_HPP__
#define __RAINDROP_GRAPHICS_BASE_FRAMEBUFFER_FULLSCREEN_QUAD_HPP__

#include "common.hpp"
#include "../Pipelines/common.hpp"

namespace Raindrop::Graphics::BaseFramebuffer{
	class FullscreenQuad{
		public:
			FullscreenQuad(Context& context);
			~FullscreenQuad();

			void render(VkCommandBuffer commandBuffer);

		private:
			Context& _context;

			VkPipelineLayout _layout;
			std::unique_ptr<Pipelines::GraphicsPipeline> _pipeline;

			VkShaderModule _fragmentModule;
			VkShaderModule _vertexModule;

			void createPipelineLayout();
			void destroyPipelineLayout();
			void createPipeline();

			void createShaderModules();
			void destroyShaderModules();
			void createFragmentModule();
			void createVertexModule();

			void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);

	};
}

#endif