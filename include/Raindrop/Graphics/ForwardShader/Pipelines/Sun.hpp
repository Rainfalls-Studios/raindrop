#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SUN_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SUN_HPP__

#include <Raindrop/Graphics/ForwardShader/Pipelines/common.hpp>

namespace Raindrop::Graphics::ForwardShader::Pipelines{
	class Sun{
		public:
			Sun(GraphicsContext& context, VkDescriptorSetLayout descriptorLayout);
			~Sun();

			GraphicsPipeline& pipeline();

		private:
			GraphicsContext& _context;
			std::shared_ptr<GraphicsPipeline> _pipeline;

			void createPipeline(VkDescriptorSetLayout descriptorLayout);

	};
}


#endif