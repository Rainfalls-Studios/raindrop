#ifndef __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_MANAGER_HPP__
#define __RAINDROP_GRAPHICS_SHADERS_GRAPHICS_PIPELINE_MANAGER_HPP__

#include <Raindrop/Graphics/Shaders/common.hpp>

namespace Raindrop::Graphics::Shaders{
	class GraphicsPipelineManager{
		public:
			GraphicsPipelineManager(Context& context);
			~GraphicsPipelineManager();

			std::shared_ptr<GraphicsPipeline> create(const GraphicsPipelineBuilder& builder);
			std::shared_ptr<GraphicsPipeline> get(const std::string& name);

			void clean();
		
		private:
			Context& _context;
			std::unordered_map<std::string, std::shared_ptr<GraphicsPipeline>> _pipelines;
	};
}

#endif