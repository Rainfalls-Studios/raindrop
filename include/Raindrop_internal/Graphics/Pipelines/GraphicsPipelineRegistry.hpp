#ifndef __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__
#define __RAINDROP_INTERNAL_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__

#include "common.hpp"
#include <Raindrop_internal/Templates/ResourceRegistry.hpp>

namespace Raindrop::Internal::Graphics::Pipelines{
	class GraphicsPipelineRegistry : public Templates::ResourceRegistry<GraphicsPipeline, GraphicsPipelineID>{
		public:
			GraphicsPipelineRegistry(Context& context);
			~GraphicsPipelineRegistry();

			GraphicsPipelineID create(const GraphicsPipelineConfigInfo& info);
			void destroy(const GraphicsPipelineID& ID);

			std::shared_ptr<GraphicsPipeline> get(const GraphicsPipelineID& ID);

		private:
			Context& _context;
	};
}

#endif