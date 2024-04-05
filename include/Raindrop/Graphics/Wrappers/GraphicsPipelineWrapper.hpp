#ifndef __RAINDROP_GRAPHICS_WRAPPERS_GRAPHICS_PIPELINE_WRAPPER_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_GRAPHICS_PIPELINE_WRAPPER_HPP__

#include "common.hpp"
#include "../Pipelines/common.hpp"

namespace Raindrop::Graphics::Wrappers{
	class GraphicsPipelineWrapper{
		public:
			using GraphicsPipelineID = Pipelines::GraphicsPipelineID;
			using GraphicsPipeline = Pipelines::GraphicsPipeline;

			GraphicsPipelineWrapper(Context& context);
			GraphicsPipelineWrapper(Context& context, const GraphicsPipelineID& ID);
			GraphicsPipelineWrapper(const GraphicsPipelineWrapper& other);

			GraphicsPipelineWrapper& operator=(const GraphicsPipelineWrapper& other);
			GraphicsPipelineWrapper& operator=(const GraphicsPipelineID& ID);
			
			~GraphicsPipelineWrapper();
		
			GraphicsPipeline& get();
			const GraphicsPipeline& get() const;
			
			GraphicsPipelineID ID() const;

			GraphicsPipeline* operator->();
			const GraphicsPipeline* operator->() const;


			VkPipeline pipeline() const;
			void bind(VkCommandBuffer commandBuffer);

			Pipelines::LayoutID layoutID() const;
			PipelineLayoutWrapper layout() const;

		private:
			Context* _context;
			GraphicsPipelineID _ID;
	};
}

#endif