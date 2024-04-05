#ifndef __RAINDROP_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__

#include "common.hpp"

namespace Raindrop::Graphics::Pipelines{
	class GraphicsPipelineRegistry{
		public:
			GraphicsPipelineRegistry(Context& context);
			~GraphicsPipelineRegistry();

			GraphicsPipelineID create(const GraphicsPipelineConfigInfo& info);
			void destroy(const GraphicsPipelineID& ID);

			GraphicsPipeline* get(const GraphicsPipelineID& ID);

		private:
			Context& _context;

			std::vector<std::unique_ptr<GraphicsPipeline>> _pipelines;
			std::queue<GraphicsPipelineID> _freeIDs;

	};
}

#endif