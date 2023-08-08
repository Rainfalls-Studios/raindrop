#ifndef __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SHADOW_LIGHT_POINT_HPP__
#define __RAINDROP_GRAPHICS_FORWARD_SHADER_PIPELINES_SHADOW_LIGHT_POINT_HPP__

#include <Raindrop/Graphics/ForwardShader/Pipelines/common.hpp>

namespace Raindrop::Graphics::ForwardShader::Pipelines{
	class ShadowLightPoint{
		public:
			ShadowLightPoint(GraphicsContext& context, VkDescriptorSetLayout descriptorLayout);
			~ShadowLightPoint();

			GraphicsPipeline& pipeline();

		private:
			GraphicsContext& _context;
			std::shared_ptr<GraphicsPipeline> _pipeline;

			void createPipeline(VkDescriptorSetLayout descriptorLayout);

	};
}

#endif