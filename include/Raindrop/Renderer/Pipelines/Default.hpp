#ifndef __RAINDROP_RENDERER_PIPELINES_DEFAULT_HPP__
#define __RAINDROP_RENDERER_PIPELINES_DEFAULT_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::Pipelines{
	class Default{
		public:
			Default(Context& context);
			~Default();

			void bind(VkCommandBuffer commandBuffer);
			VkPipelineLayout layout() const;

		private:
			Context& _context;

			VkPipelineLayout _layout;
			std::unique_ptr<GraphicsPipeline> _pipeline;

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