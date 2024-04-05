#ifndef __RAINDROP_GRAPHICS_PIPELINES_COMMON_HPP__
#define __RAINDROP_GRAPHICS_PIPELINES_COMMON_HPP__

#include "../common.hpp"

namespace Raindrop::Graphics::Pipelines{
	struct GraphicsPipelineConfigInfo;
	struct PipelineLayoutConfigInfo;

	class GraphicsPipeline;
	class PipelineLayout;
	class LayoutRegistry;

	// class ComputePipeline; TODO

	using LayoutID = std::uint32_t;
	using GraphicsPipelineID = std::uint32_t;
}

#endif