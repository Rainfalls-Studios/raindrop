#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>
#include <Raindrop/Graphics/RenderPass/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class GraphicsPipeline{
		public:
			GraphicsPipeline(Context& context, const GraphicsPipelineBuilder& builder);
			~GraphicsPipeline();

			void bind(VkCommandBuffer commandBuffer);
			VkPipeline get() const;

			PipelineLayout& lauout();
			const PipelineLayout& layout() const;

		private:
			Context& _context;
			std::vector<std::shared_ptr<Shader>> _shaders;
			std::shared_ptr<RenderPass::RenderPass> _renderPass;
			std::shared_ptr<PipelineLayout> _layout;

			VkPipeline _pipeline = VK_NULL_HANDLE;
	};
}

#endif