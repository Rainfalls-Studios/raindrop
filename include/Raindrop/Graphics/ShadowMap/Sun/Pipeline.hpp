#ifndef __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_PIPELINE_HPP__
#define __RAINDROP_GRAPHICS_SHADOW_MAP_SUN_PIPELINE_HPP__

#include <Raindrop/Graphics/ShadowMap/Sun/common.hpp>

namespace Raindrop::Graphics::ShadowMap::Sun{
	class Pipeline{
		public:
			Pipeline(GraphicsContext& context);
			~Pipeline();

			GraphicsPipeline& get() const;
		
		private:
			GraphicsContext& _context;
			std::shared_ptr<GraphicsPipeline> _pipeline;

			void createPipeline();
	};
}

#endif