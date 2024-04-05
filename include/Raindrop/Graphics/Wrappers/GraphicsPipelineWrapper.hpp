#ifndef __RAINDROP_GRAPHICS_WRAPPERS_GRAPHICS_PIPELINE_WRAPPER_HPP__
#define __RAINDROP_GRAPHICS_WRAPPERS_GRAPHICS_PIPELINE_WRAPPER_HPP__

#include "common.hpp"
#include "../Pipelines/common.hpp"
#include "../../Utils/Wrapper.hpp"

namespace Raindrop::Graphics::Wrappers{
	class GraphicsPipelineWrapper : private Utils::Wrapper<Context, Pipelines::GraphicsPipeline, Pipelines::GraphicsPipelineID, Pipelines::INVALID_GRAPHICS_PIPELINE_ID>{
		public:
			using GraphicsPipelineID = Pipelines::GraphicsPipelineID;
			using GraphicsPipeline = Pipelines::GraphicsPipeline;

			using Wrapper::Wrapper;
			using Wrapper::operator=;
			using Wrapper::operator->;
			using Wrapper::ID;

			virtual GraphicsPipeline& get() override;
			virtual const GraphicsPipeline& get() const override;

			VkPipeline pipeline() const;
			void bind(VkCommandBuffer commandBuffer);

			Pipelines::LayoutID layoutID() const;
			PipelineLayoutWrapper layout() const;
	};
}

#endif