#ifndef __RAINDROP_GRAPHICS_PIPELINES_COMMON_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Graphics::Pipelines{
	struct GraphicsPipelineConfigInfo;
	struct PipelineLayoutConfigInfo;

	class GraphicsPipeline;
	class GraphicsPipelineRegistry;
	class PipelineLayout;
	class LayoutRegistry;

	// class ComputePipeline; TODO

	using LayoutID = std::uint32_t;
	using GraphicsPipelineID = std::uint32_t;

	static constexpr LayoutID INVALID_LAYOUT_ID = (LayoutID)(~0);
	static constexpr GraphicsPipelineID INVALID_GRAPHICS_PIPELINE_ID = (GraphicsPipelineID)(~0);
}

#endif