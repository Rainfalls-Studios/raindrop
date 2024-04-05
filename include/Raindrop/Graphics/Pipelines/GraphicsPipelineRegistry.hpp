#ifndef __RAINDROP_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_GRAPHICS_PIPELINE_REGISTRY_HPP__

#include "common.hpp"
#include "../../Utils/Registry.hpp"

namespace Raindrop::Graphics::Pipelines{
	// class GraphicsPipelineRegistry{
	// 	public:
	// 		GraphicsPipelineRegistry(Context& context);
	// 		~GraphicsPipelineRegistry();

	// 		GraphicsPipelineID create(const GraphicsPipelineConfigInfo& info);
	// 		void destroy(const GraphicsPipelineID& ID);

	// 		GraphicsPipeline* get(const GraphicsPipelineID& ID);

	// 	private:
	// 		Context& _context;

	// 		std::vector<std::unique_ptr<GraphicsPipeline>> _pipelines;
	// 		std::queue<GraphicsPipelineID> _freeIDs;

	// };

	class GraphicsPipelineRegistry : private Utils::Registry<std::unique_ptr<GraphicsPipeline>, GraphicsPipelineID>{
		public:
			GraphicsPipelineRegistry(Context& context);
			~GraphicsPipelineRegistry();

			GraphicsPipelineID create(const GraphicsPipelineConfigInfo& info);
			void destroy(const GraphicsPipelineID& ID);

			GraphicsPipeline* get(const GraphicsPipelineID& ID);

			using Registry::exists;
			using Registry::inRange;

			using Registry::reservedSize;
			using Registry::freeSize;
			using Registry::usedSize;

		private:
			Context& _context;
	};
}

#endif