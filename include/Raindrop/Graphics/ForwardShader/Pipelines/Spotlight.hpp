#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SPOTLIGHT_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SPOTLIGHT_HPP__

#include <Raindrop/Graphics/ForwardShader/Pipelines/common.hpp>

namespace Raindrop::Graphics::ForwardShader::Pipelines{
	class Spotlight{
		public:
			Spotlight(GraphicsContext& context, VkDescriptorSetLayout descriptorLayout);
			~Spotlight();

			GraphicsPipeline& pipeline();

		private:
			GraphicsContext& _context;
			std::shared_ptr<GraphicsPipeline> _pipeline;

			void createPipeline(VkDescriptorSetLayout descriptorLayout);

	};
}


#endif